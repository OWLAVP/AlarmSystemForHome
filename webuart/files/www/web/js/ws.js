	/* WebSocket. */
	var ws;
	var connected = false;
/* Establish connection. */
function doConnect(addr)
{
	/* Message to be sent. */
	var msg;

	/* Do connection. */
	ws = new WebSocket(addr);

	/* Register events. */
	ws.onopen = function()
	{
		connected = true;
	};

	/* Deals with messages. */
	ws.onmessage = function (evt)
	{
		ShowData(evt.data);
		//console.log(evt.data);
	};

	/* Close events. */
	ws.onclose = function(event)
	{
		connected = false;
	};
}
