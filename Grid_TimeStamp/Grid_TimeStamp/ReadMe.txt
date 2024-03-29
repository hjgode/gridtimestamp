========================================================================
    DYNAMIC LINK LIBRARY : Grid_TimeStamp Project Overview
========================================================================

= Usage =

Use this DLL in an Intermec Device at IntermecSettings-DataCollection-VirtualWedge-GRID

An example entry:

<code>
<CDEF>\Grid_TimeStamp.DLL|TimeStamp|Compile|\t@\t@\t@\n
</code>

another xml config example:

<code>
<?xml version="1.0" encoding="UTF-8"?>
<DevInfo Action="Set" Persist="true" SeqNo="264">
 <Subsystem Name="Data Collection">
  <Group Name="Virtual Wedge">
   <Field Name="Virtual wedge">1</Field>
   <Field Name="Grid">&lt;CDEF&gt;\Windows\Grid_TimeStamp.dll|TimeStamp|Compile|\t@\t@\n</Field>
  </Group>
 </Subsystem>
</DevInfo>
</code>

You specify up to three elements behind "Compile". The parts have to be seprated by "@". 
The first of three parts will be used as separator for date and time. The second of 
three parts will be used as separator between the time stamp and the barcode data. The 
third part will be used as string sent after the barcode data.

You can also specify only one or two parts. The last part is always the postamble 
and the previous part, if any, as separator between time stamp and barcode data.

You can use \r, \n and \t inside the parts. They will be translated to there ASCII 
values for TAB, CarriageReturn and NewLine.

/////////////////////////////////////////////////////////////////////////////