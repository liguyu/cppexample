		
	Using method:
	1. Connect to the Keygoe flow module. Take the following as an example:
	   IP 192.168.11.150 PORT 9000
	   The IP and Port indicate the IP and port of the flow module.
	2. Monitor E1 statuses using the following commands:
	   monitor -o pcm: Monitor the statuses of all E1s on the system only once.
	   monitor -o pcm -t: Monitor the statuses of all E1s on the system periodically.
	3. Monitor SS7 link statuses using the following commands:
	   monitor -o ss7link: Monitor the statuses of all SS7 links on the system only once.
	   monitor -o ss7link -t: Monitor the statuses of all SS7 links on the system periodically.
	4. maintain ss7 
	  maintain -u isup -ch <DSPID-CHID> -cmd <cmd type> <DPC> <OPC> <CIC>
	  The data in "<>" must be typed.
	  -u: parameter, selecting a SS7 user type
	  -ch: parameter, selecting a channel to send the signaling; DSP ID: ID of the DSP; CHID: signaling timeslot, for example, "1¨C16"
	  -cmd: parameter, selecting a message type, supporting "blo", "ubl", "rsc", "grs", "cgb", and "cgu" currently
	  DPC: 6 characters indicate 24-bit point codes, for example "040506"; 4 characters indicate 14-bit point codes, for example, "1aaa".
	  OPC: 6 characters indicate 24-bit point codes, for example "040506"; 4 characters indicate 14-bit point codes, for example, "1aaa".
	  CIC: 4 characters, for example, "0200"
	  
	  

	
