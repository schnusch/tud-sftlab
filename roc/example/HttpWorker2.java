import tud.se.httpd.*;

public class HttpWorker2 extends HttpWorker {
	private HttpConfig  config;
	private HttpServer2 server;

	public HttpWorker2(HttpConfig config) {
		super(config);
		this.config = config;
	}

	public void setServer(HttpServer2 server) {
		this.server = server;
		this.server.setWorker(this);
	}

	@Override
	protected void afterProcessingRequest() {
		if(this.getRequestNum() >= 10) {
			HttpWorker2 new_worker = new HttpWorker2(this.config);
			new_worker.setServer(this.server);
		}
	}
}
