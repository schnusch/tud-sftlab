import java.io.*;

public class RCopy
{
	public static void main(String[] args) throws IOException
	{
		if(args.length != 2)
			System.exit(2);

		RandomAccessFile in  = null;
		FileOutputStream out = null;
		byte[] buf = new byte[4096];
		try
		{
			in  = new RandomAccessFile(args[0], "r");
			out = new FileOutputStream(args[1]);

			long len = in.length();
			long off = len - (len % buf.length);
			while(true)
			{
				in.seek(off);
				int n = in.read(buf);
				if(n > 0)
				{
					for(int a = 0, z = n; a < --z; a++)
					{
						byte tmp = buf[a];
						buf[a] = buf[z];
						buf[z] = tmp;
					}
					out.write(buf, 0, n);
				}
				if(off == 0)
					break;
				off -= buf.length;
			}
		}
		finally
		{
			if(in != null)
				in.close();
			if(out != null)
				out.close();
		}
	}
}
