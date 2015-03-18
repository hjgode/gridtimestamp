Use this DLL in an Intermec Device at IntermecSettings-DataCollection-VirtualWedge-GRID

An example entry:

"

&lt;CDEF&gt;

\Grid\_TimeStamp.DLL|TimeStamp|Compile|\t@\t@\n"

You specify up to three elements behind "Compile". The parts have to be seprated by "@". The first of three parts will be used as separator for date and time. The second of three parts will be used as separator between the time stamp and the barcode data. The third part will be used as string sent after the barcode data.
You can also specify only one or two parts. The last part is always the postamble and the previous part, if any, as separator between time stamp and barcode data.
You can use \r, \n and \t inside the parts. They will be translated to there ASCII values for TAB, CarriageReturn and NewLine.