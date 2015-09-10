#define _XOPEN_SOURCE 500  /* include pread,pwrite */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <elf.h>
#include <unistd.h>
#include <errno.h>       
#include <sys/mman.h>
#include <termios.h>
#include <sys/ioctl.h>


/* memory map for libraries */
#define MAX_NAME_LEN 256
#define MEMORY_ONLY  "[memory]"
struct mm {
	char name[MAX_NAME_LEN];
	unsigned long start, end;
};

typedef struct symtab *symtab_t;
struct symlist {
	Elf32_Sym *sym;       /* symbols */
	char *str;            /* symbol strings */
	unsigned num;         /* number of symbols */
};
struct symtab {
	struct symlist *st;    /* "static" symbols */
	struct symlist *dyn;   /* dynamic symbols */
};

static void * 
xmalloc(size_t size)
{
	void *p;
	p = malloc(size);
	if (!p) {
		printf("Out of memory\n");
		exit(1);
	}
	return p;
}

static struct symlist *
get_syms(int fd, Elf32_Shdr *symh, Elf32_Shdr *strh)
{
	struct symlist *sl, *ret;
	int rv;

	ret = NULL;
	sl = (struct symlist *) xmalloc(sizeof(struct symlist));
	sl->str = NULL;
	sl->sym = NULL;

	/* sanity */
	if (symh->sh_size % sizeof(Elf32_Sym)) { 
		//printf("elf_error\n");
		goto out;
	}

	/* symbol table */
	sl->num = symh->sh_size / sizeof(Elf32_Sym);
	sl->sym = (Elf32_Sym *) xmalloc(symh->sh_size);
	rv = pread(fd, sl->sym, symh->sh_size, symh->sh_offset);
	if (0 > rv) {
		//perror("read");
		goto out;
	}
	if (rv != symh->sh_size) {
		//printf("elf error\n");
		goto out;
	}

	/* string table */
	sl->str = (char *) xmalloc(strh->sh_size);
	rv = pread(fd, sl->str, strh->sh_size, strh->sh_offset);
	if (0 > rv) {
		//perror("read");
		goto out;
	}
	if (rv != strh->sh_size) {
		//printf("elf error");
		goto out;
	}

	ret = sl;
out:
	return ret;
}

static int
do_load(int fd, symtab_t symtab)
{
	int rv;
	size_t size;
	Elf32_Ehdr ehdr;
	Elf32_Shdr *shdr = NULL, *p;
	Elf32_Shdr *dynsymh, *dynstrh;
	Elf32_Shdr *symh, *strh;
	char *shstrtab = NULL;
	int i;
	int ret = -1;
	
	/* elf header */
	rv = read(fd, &ehdr, sizeof(ehdr));
	if (0 > rv) {
		//perror("read");
		goto out;
	}
	if (rv != sizeof(ehdr)) {
		//printf("elf error\n");
		goto out;
	}
	if (strncmp(ELFMAG, ehdr.e_ident, SELFMAG)) { /* sanity */
		//printf("not an elf\n");
		goto out;
	}
	if (sizeof(Elf32_Shdr) != ehdr.e_shentsize) { /* sanity */
		//printf("elf error\n");
		goto out;
	}

	/* section header table */
	size = ehdr.e_shentsize * ehdr.e_shnum;
	shdr = (Elf32_Shdr *) xmalloc(size);
	rv = pread(fd, shdr, size, ehdr.e_shoff);
	if (0 > rv) {
		//perror("read");
		goto out;
	}
	if (rv != size) {
		//printf("elf error");
		goto out;
	}
	
	/* section header string table */
	size = shdr[ehdr.e_shstrndx].sh_size;
	shstrtab = (char *) xmalloc(size);
	rv = pread(fd, shstrtab, size, shdr[ehdr.e_shstrndx].sh_offset);
	if (0 > rv) {
		//perror("read");
		goto out;
	}
	if (rv != size) {
		//printf("elf error\n");
		goto out;
	}

	/* symbol table headers */
	symh = dynsymh = NULL;
	strh = dynstrh = NULL;
	for (i = 0, p = shdr; i < ehdr.e_shnum; i++, p++)
		if (SHT_SYMTAB == p->sh_type) {
			if (symh) {
				//printf("too many symbol tables\n");
				goto out;
			}
			symh = p;
		} else if (SHT_DYNSYM == p->sh_type) {
			if (dynsymh) {
				//printf("too many symbol tables\n");
				goto out;
			}
			dynsymh = p;
		} else if (SHT_STRTAB == p->sh_type
			   && !strncmp(shstrtab+p->sh_name, ".strtab", 7)) {
			if (strh) {
				//printf("too many string tables\n");
				goto out;
			}
			strh = p;
		} else if (SHT_STRTAB == p->sh_type
			   && !strncmp(shstrtab+p->sh_name, ".dynstr", 7)) {
			if (dynstrh) {
				//printf("too many string tables\n");
				goto out;
			}
			dynstrh = p;
		}
	/* sanity checks */
	if ((!dynsymh && dynstrh) || (dynsymh && !dynstrh)) {
		//printf("bad dynamic symbol table");
		goto out;
	}
	if ((!symh && strh) || (symh && !strh)) {
		//printf("bad symbol table");
		goto out;
	}
	if (!dynsymh && !symh) {
		//printf("no symbol table");
		goto out;
	}

	/* symbol tables */
	if (dynsymh)
		symtab->dyn = get_syms(fd, dynsymh, dynstrh);
	if (symh)
		symtab->st = get_syms(fd, symh, strh);
	ret = 0;
out:
	free(shstrtab);
	free(shdr);
	return ret;
}

static symtab_t
load_symtab(char *filename)
{
	int fd;
	symtab_t symtab;

	symtab = (symtab_t) xmalloc(sizeof(*symtab));
	memset(symtab, 0, sizeof(*symtab));

	fd = open(filename, O_RDONLY);
	if (0 > fd) {
		//perror("open");
		return NULL;
	}
	if (0 > do_load(fd, symtab)) {
		//printf("Error ELF parsing %s\n", filename);
		free(symtab);
		symtab = NULL;
	}
	close(fd);
	return symtab;
}


static int
load_memmap(pid_t pid, struct mm *mm, int *nmmp)
{
	char raw[50000];
	char name[MAX_NAME_LEN];
	char *p;
	unsigned long start, end;
	struct mm *m;
	int nmm = 0;
	int fd, rv;
	int i;

	sprintf(raw, "/proc/%d/maps", pid);
	fd = open(raw, O_RDONLY);
	if (0 > fd) {
		//printf("Can't open %s for reading\n", raw);
		return -1;
	}

	/* Zero to ensure data is null terminated */
	memset(raw, 0, sizeof(raw));

	p = raw;
	while (1) {
		rv = read(fd, p, sizeof(raw)-(p-raw));
		if (0 > rv) {
			//perror("read");
			return -1;
		}
		if (0 == rv)
			break;
		p += rv;
		if (p-raw >= sizeof(raw)) {
			//printf("Too many memory mapping\n");
			return -1;
		}
	}
	close(fd);

	p = strtok(raw, "\n");
	m = mm;
	while (p) {
		/* parse current map line */
		rv = sscanf(p, "%08lx-%08lx %*s %*s %*s %*s %s\n",
			    &start, &end, name);

		p = strtok(NULL, "\n");

		if (rv == 2) {
			m = &mm[nmm++];
			m->start = start;
			m->end = end;
			strcpy(m->name, MEMORY_ONLY);
			continue;
		}

		/* search backward for other mapping with same name */
		for (i = nmm-1; i >= 0; i--) {
			m = &mm[i];
			if (!strcmp(m->name, name))
				break;
		}

		if (i >= 0) {
			if (start < m->start)
				m->start = start;
			if (end > m->end)
				m->end = end;
		} else {
			/* new entry */
			m = &mm[nmm++];
			m->start = start;
			m->end = end;
			strcpy(m->name, name);
		}
	}

	*nmmp = nmm;
	return 0;
}

/* Find libc in MM, storing no more than LEN-1 chars of
   its name in NAME and set START to its starting
   address.  If libc cannot be found return -1 and
   leave NAME and START untouched.  Otherwise return 0
   and null-terminated NAME. */
static int
find_libname(char *libn, char *name, int len, unsigned long *start,
	  struct mm *mm, int nmm)
{
	int i;
	struct mm *m;
	char *p;
	for (i = 0, m = mm; i < nmm; i++, m++) {
		if (!strcmp(m->name, MEMORY_ONLY))
			continue;
		p = strrchr(m->name, '/');
		if (!p)
			continue;
		p++;
		if (strncmp(libn, p, strlen(libn)))
			continue;
		p += strlen(libn);

		/* here comes our crude test -> 'libc.so' or 'libc-[0-9]' */
		if (!strncmp(".so", p, 3)) // || (p[0] == '-' && isdigit(p[1])))
			break;
	}
	if (i >= nmm)
		/* not found */
		return -1;

	*start = m->start;
	strncpy(name, m->name, len);
	if (strlen(m->name) >= len)
		name[len-1] = '\0';
		
	mprotect((void*)m->start, m->end - m->start, PROT_READ|PROT_WRITE|PROT_EXEC);
	return 0;
}

static int
lookup2(struct symlist *sl, unsigned char type,
	char *name, unsigned long *val)
{
	Elf32_Sym *p;
	int len;
	int i;

	len = strlen(name);
	for (i = 0, p = sl->sym; i < sl->num; i++, p++) {
		//printf("name: %s %x\n", sl->str+p->st_name, p->st_value);
		if (!strncmp(sl->str+p->st_name, name, len)
		    && ELF32_ST_TYPE(p->st_info) == type) {
			//if (p->st_value != 0) {
			*val = p->st_value;
			return 0;
			//}
		}
	}
	return -1;
}

static int
lookup_sym(symtab_t s, unsigned char type,
	   char *name, unsigned long *val)
{
	if (s->dyn && !lookup2(s->dyn, type, name, val))
		return 0;
	if (s->st && !lookup2(s->st, type, name, val))
		return 0;
	return -1;
}

static int
lookup_func_sym(symtab_t s, char *name, unsigned long *val)
{
	return lookup_sym(s, STT_FUNC, name, val);
}

int find_name(pid_t pid, char *name, char *libn, unsigned long *addr)
{
	struct mm mm[250];
	unsigned long libcaddr;
	int nmm;
	char libc[256];
	symtab_t s;

	if (0 > load_memmap(pid, mm, &nmm)) {
		//printf("cannot read memory map\n");
		return -1;
	}
	if (0 > find_libname(libn, libc, sizeof(libc), &libcaddr, mm, nmm)) {
		//printf("cannot find libc\n");
		return -1;
	}
	s = load_symtab(libc);
	if (!s) {
		//printf("cannot read symbol table\n");
		return -1;
	}
	if (0 > lookup_func_sym(s, name, addr)) {
		//printf("cannot find %s\n", name);
		return -1;
	}
	*addr += libcaddr;
	return 0;
}



# define IBAUD0 0

/* Set *T to indicate raw mode. */
void cfmakeraw (struct termios *t)
	{
	  t->c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
	  t->c_oflag &= ~OPOST;
	  t->c_lflag &= ~(ECHO|ECHONL|ICANON|ISIG|IEXTEN);
	  t->c_cflag &= ~(CSIZE|PARENB);
	  t->c_cflag |= CS8;
	  t->c_cc[VMIN] = 1; /* read returns when one char is available. */
	  t->c_cc[VTIME] = 0;
	}
#define __KERNEL_NCCS 19
struct __kernel_termios
	{
	    tcflag_t c_iflag; /* input mode flags */
	    tcflag_t c_oflag; /* output mode flags */
	    tcflag_t c_cflag; /* control mode flags */
	    tcflag_t c_lflag; /* local mode flags */
	    cc_t c_line; /* line discipline */
    cc_t c_cc[__KERNEL_NCCS]; /* control characters */
	};


/* Set the state of FD to *TERMIOS_P. */
int tcsetattr (int fd, int optional_actions, const struct termios *termios_p)
	{
	    struct __kernel_termios k_termios;
	    unsigned long int cmd;
	    int retval;
	
	    switch (optional_actions)
	    {
	    case TCSANOW:
	        cmd = TCSETS;
	        break;
	    case TCSADRAIN:
	        cmd = TCSETSW;
	        break;
	    case TCSAFLUSH:
	        cmd = TCSETSF;
	        break;
	    default:
	        //__set_errno (EINVAL);
	        return -1;
	    }
	
	    k_termios.c_iflag = termios_p->c_iflag & ~IBAUD0;
	    k_termios.c_oflag = termios_p->c_oflag;
	    k_termios.c_cflag = termios_p->c_cflag;
	    k_termios.c_lflag = termios_p->c_lflag;
	    k_termios.c_line = termios_p->c_line;
	#ifdef _HAVE_C_ISPEED
    k_termios.c_ispeed = termios_p->c_ispeed;
	#endif
	#ifdef _HAVE_C_OSPEED
	    k_termios.c_ospeed = termios_p->c_ospeed;
	#endif
	    memcpy (&k_termios.c_cc[0], &termios_p->c_cc[0],
	        __KERNEL_NCCS * sizeof (cc_t));
	
	    retval = ioctl (fd, cmd, &k_termios);
	
	    if (retval == 0 && cmd == TCSETS)
	    {
	    /* The Linux kernel has a bug which silently ignore the invalid
	       c_cflag on pty. We have to check it here. */
	    int save = 0; //errno;
	    retval = ioctl (fd, TCGETS, &k_termios);
	    if (retval)
	    {
	        /* We cannot verify if the setting is ok. We don't return
	           an error (?). */
	        //__set_errno (save);
	        retval = 0;
	    }
	    else if ((termios_p->c_cflag & (PARENB | CREAD))
	        != (k_termios.c_cflag & (PARENB | CREAD))
	        || ((termios_p->c_cflag & CSIZE)
	            && ((termios_p->c_cflag & CSIZE)
	            != (k_termios.c_cflag & CSIZE))))
	    {
	        /* It looks like the Linux kernel silently changed the
	           PARENB/CREAD/CSIZE bits in c_cflag. Report it as an
	           error. */
	        //__set_errno (EINVAL);
	        retval = -1;
	    }
	    }
	
	    return retval;
}

int tcgetattr (int fd, struct termios *termios_p)
	{
	    struct __kernel_termios k_termios;
	    int retval;
	
	    retval = ioctl (fd, TCGETS, &k_termios);
	    if(retval == 0) {
	        termios_p->c_iflag = k_termios.c_iflag;
	        termios_p->c_oflag = k_termios.c_oflag;
	        termios_p->c_cflag = k_termios.c_cflag;
	        termios_p->c_lflag = k_termios.c_lflag;
	        termios_p->c_line = k_termios.c_line;
	#ifdef _HAVE_C_ISPEED
	        termios_p->c_ispeed = k_termios.c_ispeed;
	#endif
	#ifdef _HAVE_C_OSPEED
	        termios_p->c_ospeed = k_termios.c_ospeed;
	#endif
	
	
	        if (sizeof (cc_t) == 1 || _POSIX_VDISABLE == 0
	            || (unsigned char) _POSIX_VDISABLE == (unsigned char) -1)
	        {
			#if 0
	        memset (mempcpy (&termios_p->c_cc[0], &k_termios.c_cc[0],
	                __KERNEL_NCCS * sizeof (cc_t)),
	            _POSIX_VDISABLE, (NCCS - __KERNEL_NCCS) * sizeof (cc_t));
			#endif
	        memset ( (memcpy (&termios_p->c_cc[0], &k_termios.c_cc[0],
	                __KERNEL_NCCS * sizeof (cc_t)) + (__KERNEL_NCCS * sizeof (cc_t))) ,
	            _POSIX_VDISABLE, (NCCS - __KERNEL_NCCS) * sizeof (cc_t));
	
	        } else {
	        size_t cnt;
	
	        memcpy (&termios_p->c_cc[0], &k_termios.c_cc[0],
	            __KERNEL_NCCS * sizeof (cc_t));
	
	        for (cnt = __KERNEL_NCCS; cnt < NCCS; ++cnt)
	            termios_p->c_cc[cnt] = _POSIX_VDISABLE;
	        }
	    }
	
	    return retval;
	}

