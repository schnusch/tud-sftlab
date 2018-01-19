import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import tud.se.httpd.HttpConfig;
import tud.se.httpd.HttpException;

/*
 * File: SimpleMain.java
 * Created on 01.12.2004 by: suesskraut
 *
 *
 */

/**
 * <p>Simple demonstration of how to compose the HttpServer and the HttpWorker
 * to a simple webserver.</p>
 * @author suesskraut
 * <pre>part of: tiny-http package: 
 * File: SimpleMain.java
 * created: 01.12.2004</pre>
 */
public class SimpleMain
{
	public static void main (String[] args) throws IOException, HttpException
	{
		// parse command line arguments
		HttpConfig config = new HttpConfig (args);

		// creating the worker
		HttpWorker2 worker = new HttpWorker2(config);

		while(true) {
			// creating the server
			HttpServer2 server = new HttpServer2(config);
			// connecting server and worker
			worker.setServer(server);
			// start the server
			server.start();

			while(true) {
				try {
					server.join();
				} catch(InterruptedException e) {
					continue;
				}
				break;
			}

			worker = server.getWorker();
		}
	}
}
