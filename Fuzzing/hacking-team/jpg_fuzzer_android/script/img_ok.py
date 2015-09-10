import json
import sys
import os
import time
import requests
from PIL import Image
from StringIO import StringIO
from requests.exceptions import ConnectionError
 
def go(query, path):
  """Download full size images from Google image search.
 
  Don't print or republish images without permission.
  I used this to train a learning algorithm.
  """
  BASE_URL = 'https://ajax.googleapis.com/ajax/services/search/images?'\
             'v=1.0&q=' + query + '&start=%d'
 
  BASE_PATH = os.path.join(path, query)

  log = open("./log", 'a')
 
  if not os.path.exists(BASE_PATH):
    os.makedirs(BASE_PATH)
 
  start = 0 # Google's start query string parameter for pagination.
  while start < 60: # Google will only return a max of 56 results.
    r = requests.get(BASE_URL % start)
    for image_info in json.loads(r.text)['responseData']['results']:
      url = image_info['unescapedUrl']
      try:
        print url
        log.write(url +  "\n")
        #image_r = requests.get(url)
      except ConnectionError, e:
        print 'could not download %s' % url
        continue
 
    '''  # Remove file-system path characters from name.
      title = image_info['titleNoFormatting'].replace('/', '').replace('\\', '')
 
      file = open(os.path.join(BASE_PATH, '%s.jpg') % title, 'w')
      try:
        Image.open(StringIO(image_r.content)).save(file, 'JPEG')
      except IOError, e:
        # Throw away some gifs...blegh.
        print 'could not save %s' % url
        continue
      finally:
        file.close()'''
 
    print start
    start += 4 # 4 images per page.
 
    # Be nice to Google and they'll be nice back :)
    time.sleep(1.5)


if __name__ == '__main__':
 
  #keyw = [ "320x200 png", "logo png", "640x480 png", "transparency png", 
  #keyw = ["wallpaper png", "500x500 png", \
  keyw = [ '''"icon png", ''' "3d png", "android png" , "map small png" , "texture png", "400x400 png", "wallpaper full hd png", \
            "wallpaper hd ready png", "wall texture small png", "bump 100x100 png", "300x270 png", "250x250 png"]

  for i in keyw:
    go( i , 'myDirectory')
    print "Going to sleep ....\n"
    time.sleep(10)
