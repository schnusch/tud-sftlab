import tud.se.httpd.*;

public class HttpServer2 extends HttpServer {
	private HttpWorker2 worker;

	public HttpServer2(HttpConfig config) {
		super(config);
	}

	public void setWorker(HttpWorker2 worker) {
		super.setWorker(worker);
		this.worker = worker;
	}

	public HttpWorker2 getWorker() {
		return this.worker;
	}

	@Override
	protected void afterHandlingRequest() {
		if(this.getReceivedRequests() >= 15)
			this.halt();
	}
}
