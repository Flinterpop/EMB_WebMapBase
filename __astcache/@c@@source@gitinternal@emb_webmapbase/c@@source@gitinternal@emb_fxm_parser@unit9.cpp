//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit9.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm9 *Form9;


/* prototype of the packet handler */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

void ParseFIM(char * ips, byte * FxM_payload);
void ParseFOM(char * ips, byte * FxM_payload);

BOOL LoadNpcapDlls()
{
	_TCHAR npcap_dir[512];
	UINT len;
	len = GetSystemDirectory(npcap_dir, 480);
	if (!len) {
		fprintf(stderr, "Error in GetSystemDirectory: %x",(unsigned int) GetLastError());
		return FALSE;
	}
	_tcscat_s(npcap_dir, 512, _T("\\Npcap"));
	if (SetDllDirectory(npcap_dir) == 0) {
		fprintf(stderr, "Error in SetDllDirectory: %x", (unsigned int) GetLastError());
		return FALSE;
	}
	return TRUE;
}

#include <stdio.h>
#include <map>

#define IPTOSBUFFERS    12
char *iptos(u_long in)
{
    static char output[IPTOSBUFFERS][3*4+3+1];
    static short which;
    u_char *p;

    p = (u_char *)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	//snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]),"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	sprintf(output[which],"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output[which];
}





void __fastcall TForm9::BN_LoadAdapterListClick(TObject *Sender)
{
	LoadAdapterList();
}
//---------------------------------------------------------------------------


int TForm9::LoadAdapterList()
{
	ipMap.clear();
	LB_Adapters->Items->Clear();
	numAdapters = 0;

	pcap_if_t *dev;
	int inum;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list */
	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		pmeF("Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}

	/* Print the list */
	//alldevs is a  pcap_if_t *    which are data structures in a linked list
	int index=0;
	for(dev=alldevs; dev; dev=dev->next)
	{
		pmeF("%d. %s", ++numAdapters, dev->name);
		if (dev->description) pmeF(" (%s)", dev->description);
		else pmeF(" (No description available)");
		pmeF("\tLoopback: %s",(dev->flags & PCAP_IF_LOOPBACK)?"yes":"no");
		pmeF("\tWireless: %s",(dev->flags & PCAP_IF_WIRELESS)?"yes":"no");
		pmeF("\tUp: %s",(dev->flags & PCAP_IF_UP)?"yes":"no");
		pmeF("\tRunning: %s",(dev->flags & PCAP_IF_RUNNING)?"yes":"no");

		if (dev->flags & PCAP_IF_WIRELESS) continue;
		if (!(dev->flags & PCAP_IF_UP)) continue;
		if (!(dev->flags & PCAP_IF_RUNNING)) continue;


		/* find the IP addresses for this device*/
		char ip6str[128];
		String ip;
		bool hasAtLeastOneIP=false;
		for(pcap_addr_t *a = dev->addresses; a; a = a->next) {
			if (a->addr->sa_family!= AF_INET) continue;    //not IPV4
			if (a->addr)
				{
					pmeF("\tAddress: %s",iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
					hasAtLeastOneIP = true;
					ip = iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr);

					pme(ip);
					ipMap.insert(std::pair<int,String>(index,ip));
				}
			//break;
			}
		if(hasAtLeastOneIP)	++index;

		//populate the adapters list box with adapters that have at least one IPv4 address
		char buf[100];
		sprintf(buf,"%d %s",numAdapters,dev->name);
		if (hasAtLeastOneIP)
		{
			LB_Adapters->Items->Add(buf);
			IPAddressList.push_back(ip);
		}
	}

	if(numAdapters==0)
	{
		pmeF("\nNo interfaces found! Make sure Npcap is installed.\n");
		return -1;
	}

	else
	{
		pmeF("\nFound %d interfaces of which %d are usable.",numAdapters,LB_Adapters->Items->Count);
	}
	//LB_Adapters->ItemIndex=0;      //select an interface
	return 0;
}


void __fastcall TForm9::LB_AdaptersCDBLlick(TObject *Sender)
{
	if (LB_Adapters->Items->Count < 1 ) return;
	SelectAdapter();
}

void __fastcall TForm9::SelectAdapter()
{
	//if (LB_Adapters->Items->Count < 1 ) return;
	int selIndex = LB_Adapters->ItemIndex;
	pmeF("START: Selected List Box index is: %d", selIndex);    //ItemIndex is 0 through num listbox entries minus 1

	String st = LB_Adapters->Items->Strings[selIndex];
	SelectedAdapterNumber = StrToInt(st.c_str()[0]); //needed by start capture

	LB_SelectedIP->Caption ="";

	for(std::pair<int, String> elem : ipMap)
	{
		//pmeF("Found IP: %d",elem.first);
		if (elem.first==selIndex) {
			pmeF("Found IP");
			pme(elem.second);
			LB_SelectedIP->Caption = LB_SelectedIP->Caption + elem.second + "\r\n";

		}
	}
}
//---------------------------------------------------------------------------



void __fastcall TForm9::LB_AdaptersDblCDBLlick(TObject *Sender)
{
	if (LB_Adapters->Items->Count < 1 ) return;
	SelectAdapter();
	StartCapture();
}
//---------------------------------------------------------------------------







//---------------------------------------------------------------------------
__fastcall TForm9::TForm9(TComponent* Owner)
	: TForm(Owner)
{
	LoadAdapterList();
}
//---------------------------------------------------------------------------





int bg_ntohs(int netshort)
{
	int a = (netshort & 0xFF00);
	int b = (netshort & 0x00FF);
	return (b << 8) + (a >> 8);
}


void HexDump(byte* d, int l)
{
	for (int x = 0;x < l;x++)
	{
		printf("%02X ", d[x]);
		if (!(x+1) % 16) printf("\r\n");
	}
}
static int numPackets = 0;





void  TForm9::pme_SetColour(TColor color, bool _isBold)
{
	colour = color;
	isBold = _isBold;
}

void  TForm9::_pme(const char* fmt)
{
	TM_Debug->StyleElements = TM_Debug->StyleElements >> seFont;  //necessary to allow colour/font changes if a style is in use
	TM_Debug->SelAttributes->Color = colour;
	if (isBold) TM_Debug->SelAttributes->Style = Form9->TM_Debug->SelAttributes->Style << fsBold;
	else TM_Debug->SelAttributes->Style = Form9->TM_Debug->SelAttributes->Style >> fsBold;
	TM_Debug->SelText = fmt;
	//TM_Debug->SelText = "\r\n";

	SendMessage(TM_Debug->Handle, WM_VSCROLL, SB_BOTTOM, 0);
}

void  TForm9::_pme(String st)
{
	TM_Debug->StyleElements = TM_Debug->StyleElements >> seFont; //necessary to allow colour/font changes if a style is in use
	TM_Debug->SelAttributes->Color = colour;
	if (isBold) TM_Debug->SelAttributes->Style = Form9->TM_Debug->SelAttributes->Style << fsBold;
	else TM_Debug->SelAttributes->Style = Form9->TM_Debug->SelAttributes->Style >> fsBold;
	TM_Debug->SelText = st;
	//TM_Debug->SelText = "\r\n";

	SendMessage(TM_Debug->Handle, WM_VSCROLL, SB_BOTTOM, 0);
}



//this is a printf for the Debug Memo widget
void  TForm9::pmeF(const char* fmt, ...)
{
	if (TM_Debug->Lines->Count > 1000) TM_Debug->Lines->Clear();

	va_list args;
	va_start(args, fmt);
	char buf[200];
	vsprintf(buf,fmt,args);
	_pme(buf);
	va_end(args);
}


void  TForm9::pme(const char* fmt)
{
	if (TM_Debug->Lines->Count > 1000) TM_Debug->Lines->Clear();
	_pme(fmt);
}

void  TForm9::pme(String st)
{
	if (TM_Debug->Lines->Count > 1000) TM_Debug->Lines->Clear();
	_pme(st);

}



void __fastcall TForm9::BN_StartCaptureClick(TObject *Sender)
{
	StartCapture();
}

void __fastcall TForm9::StartCapture()
{
	pmeF("Trying to open adapter # %d",SelectedAdapterNumber);

	if(SelectedAdapterNumber < 1 || SelectedAdapterNumber > numAdapters)
	{
		pmeF("\nInterface number out of range.\n");
		pcap_freealldevs(alldevs);
		numAdapters=0;
		return;
	}


	pcap_if_t *dev;
	/* Jump to the selected adapter */
	int i=0;
	for(dev=alldevs, i=0; i< SelectedAdapterNumber-1 ;dev=dev->next, i++);    //run through the linked list

	if (dev->flags & PCAP_IF_LOOPBACK) isLoopbackAdapter=true;
	else isLoopbackAdapter=false;

	char errbuf[PCAP_ERRBUF_SIZE];
	if ((adhandle= pcap_open_live(dev->name,// name of the device
							 65536,			// portion of the packet to capture.
											// 65536 grants that the whole packet will be captured on all the MACs.
							 1,				// promiscuous mode (nonzero means promiscuous)
							 1000,			// read timeout
							 errbuf			// error buffer
							 )) == NULL)
	{
		pmeF("\nUnable to open the adapter. %s is not supported by Npcap\n", dev->name);
		pcap_freealldevs(alldevs);
		numAdapters=0;
		return;
	}

	pmeF("\nlistening on %s...\n", dev->description);

	//compile the filter
	int netmask = 0xffffff;
	struct bpf_program fcode;
	char buf[100];
	PlatformDPort = StrToInt(TE_Port->Text);
	sprintf(buf,"port %d",PlatformDPort);
	pmeF(buf);

	if (pcap_compile(adhandle, &fcode, buf, 1, netmask) < 0)
	{
		pmeF("\nUnable to compile the packet filter. Check the syntax.\n");
		pcap_freealldevs(alldevs);
		numAdapters=0;
		return;
	}

	if (pcap_setfilter(adhandle, &fcode) < 0)
	{
		pmeF("\nError setting the filter.\n");
		pcap_freealldevs(alldevs);
		numAdapters=0;
		return;
	}

	TaskRunning=true;
	tasks[0] = TTask::Create([&](){
		while(TaskRunning)
		{
			//Label1->Caption=String(Random(100));

			//int c = pcap_loop(adhandle, 0, packet_handler, NULL);
			//  OR
			int c = pcap_dispatch(adhandle, 0, packet_handler, NULL);

			//pmeF("pcap_loop Returned with %d\r\n", c);
		}
		pmeF("Listen Thread Ended...\r\n");
		pcap_close(adhandle);
	});
	tasks[0]->Start();
	pmeF("Listen Thread Started...\r\n");

	BN_StartCapture->Enabled=false;
	BN_StopCapture->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TForm9::TB_QUITClick(TObject *Sender)
{
	Close();
}

void __fastcall TForm9::FormClose(TObject *Sender, TCloseAction &Action)
{
	TaskRunning = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm9::BN_StopCaptureClick(TObject *Sender)
{
   TaskRunning = false;
	BN_StartCapture->Enabled=true;
	BN_StopCapture->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm9::BN_ClearClick(TObject *Sender)
{
	TM_Debug->Lines->Clear();
}
//---------------------------------------------------------------------------

String decToOctal(int n)
{
	// Array to store octal number
	int octalNum[100];

	// Counter for octal number array
	int i = 0;
	while (n != 0) {
		// Storing remainder in octal array
		octalNum[i] = n % 8;
		n = n / 8;
		i++;
	}

	// Printing octal number array in
	// reverse order
	String s = "";
	for (int j = i - 1; j >= 0; j--)
	{
		//cout << octalNum[j];
		s+=octalNum[j];
	}

	return s;
}



void decToOctal(int n, char *buf)
{
	// Array to store octal number
	int octalNum[100];

	// Counter for octal number array
	int i = 0;
	while (n != 0) {
		// Storing remainder in octal array
		octalNum[i] = n % 8;
		n = n / 8;
		i++;
	}

	// Printing octal number array in
	// reverse order
	String s = "";
	int a=0;
	for (int j = i - 1; j >= 0; j--)
	{
		//cout << octalNum[j];
		s+=octalNum[j];
		buf[a++] = 0x30 + octalNum[j];
	}
    buf[a]=0;
}






//First step in parsing a VMF message presented as a byte array
std::vector<bool> *BuildBitStringFromByteArray(const unsigned char* m, int len)
{
	std::vector<bool> *bitstring = new std::vector<bool>;

	for (int i = 0; i < len; i++) {
		byte cb = m[i];
		for (int x=0; x < 8; x++) {
			if (cb & (0b00000001 <<x)) {
				bitstring->push_back(true);
			}
			else bitstring->push_back(false);
		}
	}
	return bitstring;
}


void TForm9::PrintBitString(std::vector<bool> *bitstring)
{
	pmeF("bitstring is length %d",bitstring->size());
	std::string  s = "";
	for (int i=0; i<bitstring->size();i++) {
		 if (bitstring->at(i)) s.append("1");
			else s.append("0");
			if (!((i+1)%8)) s.append(" ");

	}
	s.append("\r\n");
	TM_Debug->Lines->Append(s.c_str());
}


void TForm9::addOrUpdateJMsg(int Label, int SubLabel)
{
	for (JMsg *m : JMsgList)
	{
		if ((m->Label == Label) & (m->SubLabel == SubLabel)) {
			m->count++;
			DisplayMsgList();
			return;
		}
	}
	//gets here if its a new Msg

	JMsg *_JMsg = new JMsg;
	_JMsg->Label = JSeriesLabel;
	_JMsg->SubLabel = JSeriesSubLabel;
	_JMsg->count=1;
	JMsgList.push_back(_JMsg);
	DisplayMsgList();
}

void TForm9::DisplayMsgList()
{
	TM_MsgList->Clear();

	TM_MsgList->Lines->Add("J Msg  Count");
	TM_MsgList->Lines->Add("------------");

	for (JMsg *m : JMsgList)
	{
		char buf[30];
		sprintf(buf," J%d.%d   %04d",m->Label, m->SubLabel, m->count);
		TM_MsgList->Lines->Add(buf);
	}

}





void TForm9::addOrUpdateJU(int stn)
{
	for (track *t : JU_List)
	{
		if (t->STN == stn) {
			t->count++;
			DisplayJUList();
			return;
		}
	}
	//gets here if its a new track
	track *newT = new track;;
	newT->STN = stn;
	newT->count = 1;
	JU_List.push_back(newT);
	DisplayJUList();
}

void TForm9::DisplayJUList()
{
	TM_JUList->Clear();

	TM_JUList->Lines->Add("  JU   Count");
	TM_JUList->Lines->Add("------------");

	for (track *t : JU_List)
	{
		char _stn[10];
		decToOctal(t->STN,_stn);
		char buf[30];
		sprintf(buf,"%5s  %3d", _stn, t->count);
		TM_JUList->Lines->Add(buf);
	}

}



void TForm9::addOrUpdateTrack(int stn, int tNum)
{
	for (track *t : Track_List)
	{
		if (t->JU == tNum) {
			t->count++;
			DisplayTrackList();
			return;
		}
	}
	//gets here if its a new track
	track *newT = new track;;
	newT->STN = stn;
	newT->JU = tNum;
	newT->count = 1;
	Track_List.push_back(newT);
	DisplayTrackList();
}

void TForm9::DisplayTrackList()
{
	TM_TrackList->Clear();

	TM_TrackList->Lines->Add("  STN  Track Count");
	TM_TrackList->Lines->Add("-------------------");

	for (track *t : Track_List)
	{
		char _stn[10];
		decToOctal(t->STN,_stn);
		char _tNum[10];
		decToOctal(t->JU,_tNum);

		char buf[30];
		sprintf(buf,"%5s %5s   %3d", _stn, _tNum, t->count);
		TM_TrackList->Lines->Add(buf);
	}

}




/* Callback function invoked by libpcap for every incoming packet */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{

	(VOID)(param);
	(VOID)(pkt_data);

	ip_header* ih;
	tcp_header* th;
	u_int ip_len;
	u_short sport, dport;

	byte* FxM_payload;

	/* retrieve the position of the ip header */
	if (Form9->isLoopbackAdapter) 	ih = (ip_header*)(pkt_data + 4); //length of loopback header
	else ih = (ip_header*)(pkt_data + 14); //length of ethernet header

	int packetLength = bg_ntohs(ih->tlen);

	if (ih->proto != 6)
	{
		Form9->pme("Not TCP");
		return;
	}

	/* retrieve the position of the tcp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	th = (tcp_header*)((u_char*)ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = bg_ntohs(th->src_port);
	dport = bg_ntohs(th->dst_port);

	/* print ip addresses and udp ports */
	char buf[100];
	sprintf(buf,"  %d.%d.%d.%d:%05d -> %d.%d.%d.%d:%05d",
		ih->saddr.byte1,
		ih->saddr.byte2,
		ih->saddr.byte3,
		ih->saddr.byte4,
		sport,
		ih->daddr.byte1,
		ih->daddr.byte2,
		ih->daddr.byte3,
		ih->daddr.byte4,
		dport);

	/* retrieve the position of the FxM payload*/
	ip_len = (ih->ver_ihl & 0xf) * 4;
	int th_len = sizeof(tcp_header);
	FxM_payload = (byte *)((u_char*)ih + ip_len + th_len);

	//ip.length is total length of IP header, next header (ie TCP/UDP etc) and payload. Does not include Ethernet or Loopback length
	if (packetLength <= (ip_len + th_len))
	{
		//puts("No Payload");
		return;
	}


	Form9->PacketCount++;

	if (Form9->CB_TimeStamp->Checked==true) {
    	struct tm *ltime;
		char timestr[16];
		time_t local_tv_sec;
		/* convert the timestamp to readable format */
		local_tv_sec = header->ts.tv_sec;
		ltime = localtime(&local_tv_sec);
		strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

		Form9->pme_SetColour(clBlack,false);
		String output = IntToStr(Form9->PacketCount)+ ": ";
		output += timestr;	output += buf;
		Form9->pme(output);
	}


	if (sport==Form9->PlatformDPort)  //FOM -> message is coming from the terminal
	{
		ParseFOM(buf,FxM_payload);
	}
	else //FIM -> message is going to the terminal
	{
		ParseFIM(buf,FxM_payload);
	}

}


void ParseFIM(char * ips,byte * FxM_payload)
{
	//Parse FxM (Presentation) header
	int FIM_LastField = (FxM_payload[0] & 0x80)?1:0;
	int FIM_ID = (FxM_payload[0] & 0x7F) >> 1;

	int FxM_Length = FxM_payload[1];  //this is length in 16 bit words
	int extra = (FxM_payload[0] & 0x01);
	FxM_Length += extra  * 256;

	Form9->pme_SetColour(clRed,false);

	if (FIM_ID == 0x01)
	{
		int TransmitMethod = (FxM_payload[12] & 0x60)>>1;
		Form9->npg = FxM_payload[15] + FxM_payload[14]*256;
		Form9->stn = FxM_payload[17] + (FxM_payload[16] & 0x7F) *256;  //this is octal


		if (Form9->CB_Debug->Checked == true)
		{
			Form9->pmeF("FIM 1:");
			Form9->pmeF(ips);
			Form9->pmeF("  FIM Presentation Header: FxM Length: %02d FxM ID: %02d  Last Field Ind: %d",FxM_Length, FIM_ID, FIM_LastField);
			char _stn[10];
			decToOctal(Form9->stn,_stn);
			Form9->pmeF("  TML %d  NPG: %d  STN: %d  %s",TransmitMethod,Form9->npg, Form9->stn, _stn);
		}
		byte *JSeries =  &FxM_payload[18];
		int JLength = 2 * (FxM_Length - 8); //want length in bytes not including the FIM header
		if (!FIM_LastField) return;  //RESEARCH THIS
		Form9->ParseJ_Series(JSeries,JLength);

	}
	else  // Not FIM01
	{
		if (Form9->CB_FxM01Only->Checked == false) Form9->pmeF("  FIM%02d",FIM_ID);
	}

}


void ParseFOM(char * ips,byte * FxM_payload)
{
	//Parse FxM (Presentation) header
	int FOM_LastField = (FxM_payload[0] & 0x80)?1:0;
	int FOM_ID = (FxM_payload[0] & 0x7F) >> 1;

	int FxM_Length = FxM_payload[1];  //this is length in 16 bit words
	int extra = (FxM_payload[0] & 0x01);
	FxM_Length += extra  * 256;

	Form9->pme_SetColour(clBlue,false);

	if (FOM_ID == 0x01)
	{
		int RxMode =FxM_payload[2]; //(FxM_payload[2] & 0x80)>>7;
		Form9->stn = FxM_payload[5] + (FxM_payload[4] & 0x7F) * 256;  //this is octal
		Form9->npg = FxM_payload[7] + (FxM_payload[4] & 0x01) * 256;  //assumes   RxMode is 0

		if (Form9->CB_Debug->Checked == true)
		{
			Form9->pmeF("FOM 1:");
			Form9->pmeF(ips);
			Form9->pmeF("  FIM Presentation Header: FxM Length: %02d FxM ID: %02d  Last Field Ind: %d",FxM_Length, FOM_ID, FOM_LastField);
			Form9->pmeF("  RxMode %d  NPG: %d  STN: %d",RxMode,Form9->npg, Form9->stn);
		}

		byte *JSeries =  &FxM_payload[12];
		int JLength = 2 * (FxM_Length - 5); //want length in bytes not including the FIM header     **********
		if (!FOM_LastField) return;  //RESEARCH THIS

		Form9->ParseJ_Series(JSeries,JLength);

	}
	else //Not FOM 01
	{
		if (Form9->CB_FxM01Only->Checked == false) Form9->pmeF("  FOM%02d", FOM_ID);
	}
}


void TForm9::ParseJ_Series(byte * buf, int bufLen)
{
	char newBuf[200];
	//assume bufLen is even -> VERIFY
	for (int i= 0; i < bufLen; i+=2) {  //this reverses the byte order for each pair of bytes
		newBuf[i]=buf[i+1];
		newBuf[i+1]=buf[i];
	}

	curBit=0;
	bitstring = BuildBitStringFromByteArray((const unsigned char *)newBuf, bufLen);
	//pmeF("Bit string is %d long",bitstring->size());
	//PrintBitString(bitstring);

	int WordFormat = calcValue(2,bitstring,curBit); //00 is J2.2I, 10 is J2.2E0, 01 is Continuation word with CWord humber in another field
	if (WordFormat != 0x00) pmeF("**********Error Not intial word");
	JSeriesLabel = calcValue(5,bitstring,curBit);
	JSeriesSubLabel = calcValue(3,bitstring,curBit);
	if (CB_Debug->Checked) pmeF("    Parsing J%d.%d",JSeriesLabel, JSeriesSubLabel);

	switch (JSeriesLabel) {
		case 2:
			ParsePPLI(JSeriesSubLabel);
			break;
		case 3:
			ParseTrack(JSeriesSubLabel);
			break;
		case 12:
			//ParseTarget(JSeriesSubLabel);
			break;
		default:
			break;
	}

	addOrUpdateJMsg(JSeriesLabel, JSeriesSubLabel);


}





