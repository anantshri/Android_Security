
//
// Copyright (c) Michael Eddington
//
// Permission is hereby granted, free of charge, to any person obtaining a copy 
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
// copies of the Software, and to permit persons to whom the Software is 
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in	
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// Authors:
//   Michael Eddington (mike@dejavusecurity.com)

using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Threading;

using Peach.Core.Dom;
using Peach.Core.IO;

using NLog;

namespace Peach.Core.Publishers
{
	[Publisher("FilePdu", true)]
	[Publisher("FileStream")]
	[Publisher("file.FileWriter")]
	[Publisher("file.FileReader")]
	[Parameter("FileName", typeof(string), "Name of file to open for reading/writing")]
	[Parameter("Overwrite", typeof(bool), "Replace existing file? [true/false, default true]", "true")]
	[Parameter("Append", typeof(bool), "Append to end of file [true/false, default flase]", "false")]
	public class FilePublisherPdu : StreamPublisher
	{
		private static NLog.Logger logger = LogManager.GetCurrentClassLogger();
		protected override NLog.Logger Logger { get { return logger; } }
	        public string FileName { get; set; }

		public string fileTemplate { get; set; }
		public bool Overwrite { get; set; }
	        public bool Append { get; set; }
	        public uint Counter = 10000;
	  

		private static int maxOpenAttempts = 10;
		private FileMode fileMode = FileMode.OpenOrCreate;

		public FilePublisherPdu(Dictionary<string, Variant> args)
			: base(args)
		{
		        fileTemplate = FileName;
		  
			setFileName(0);

			if (Overwrite && Append)
				throw new PeachException("File publisher does not support Overwrite and Append being enabled at once.");
			else if (Overwrite)
				fileMode = FileMode.Create;
			else if (Append)
				fileMode = FileMode.Append | FileMode.OpenOrCreate;
			else
				fileMode = FileMode.OpenOrCreate;
		}
	  
	  
	

          protected void setFileName(uint iteration)
	      {
	    
		FileName = fileTemplate + iteration.ToString();

		if (IsControlIteration)
		  FileName += ".Control";
	      }


          protected override void OnOpen()
		{
			System.Diagnostics.Debug.Assert(stream == null);

			int i = 0;

			while (true)
			{
				try
				{
					stream = System.IO.File.Open(FileName, fileMode);
					return;
				}
				catch (Exception ex)
				{
					if (++i < maxOpenAttempts)
					{
						Thread.Sleep(200);
					}
					else
					{
						Logger.Error("Could not open file '{0}' after {1} attempts.  {2}", FileName, maxOpenAttempts, ex.Message);
						throw new SoftException(ex);
					}
				}
			}
		}

		protected override void OnClose()
		{
			System.Diagnostics.Debug.Assert(stream != null);

			try
			{
				stream.Close();
			}
			catch (Exception ex)
			{
				logger.Error(ex.Message);
			}

			stream = null;
		}

		protected override void OnOutput(BitwiseStream data)
		{
		  
		  int byteRead;
		  System.Text.ASCIIEncoding encoding= new System.Text.ASCIIEncoding();
		  Byte[] bytes;
		  long data_length = data.Length;

		  if((this.Iteration % Counter) == 0)
		    {
		      this.OnClose();
		      this.setFileName(this.Iteration / Counter);
		      this.OnOpen();

		    }

		  
		  if(data_length == 0) return;
		  
		  while((byteRead = data.ReadByte()) != -1)
		    {
                      bytes = encoding.GetBytes(byteRead.ToString("X2"));
		      stream.WriteByte(bytes[0]);
		      stream.WriteByte(bytes[1]);
		    }

		  bytes = encoding.GetBytes("\n");
		  stream.WriteByte(bytes[0]);


		}
	}
}


// END
