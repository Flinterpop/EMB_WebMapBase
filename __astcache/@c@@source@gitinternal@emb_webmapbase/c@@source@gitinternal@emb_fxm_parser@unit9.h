//---------------------------------------------------------------------------

#ifndef Unit9H
#define Unit9H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Unit1.h"

#include <System.Threading.hpp>

#include <pcap.h>
#include <Vcl.ComCtrls.hpp>

#include <vector>
#include <map>


/* 4 bytes IP address */
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header {
	u_char  ver_ihl; // Version (4 bits) + IP header length (4 bits)
	u_char  tos;     // Type of service
	u_short tlen;    // Total length
	u_short identification; // Identification
	u_short flags_fo; // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;      // Time to live
	u_char  proto;    // Protocol
	u_short crc;      // Header checksum
	ip_address  saddr; // Source address
	ip_address  daddr; // Destination address
	u_int  op_pad;     // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header {
	u_short sport; // Source port
	u_short dport; // Destination port
	u_short len;   // Datagram length
	u_short crc;   // Checksum
}udp_header;


typedef struct tcp_header {
	uint16_t src_port;
	uint16_t dst_port;
	uint32_t seq;
	uint32_t ack;
	uint8_t  data_offset;  // 4 bits
	uint8_t  flags;
	uint16_t window_size;
	uint16_t checksum;
	uint16_t urgent_p;
} tcp_header;


typedef struct _track {
	uint16_t STN;
	uint16_t JU;
	uint32_t count;

} track;


typedef struct _JMsg {
	uint16_t Label;
	uint16_t SubLabel;
    char JName[10];
	uint32_t count;

} JMsg;


//---------------------------------------------------------------------------
class TForm9 : public TForm
{
__published:	// IDE-managed Components
	TEdit *TE_Port;
	TButton *BN_LoadAdapterList;
	TButton *TB_QUIT;
	TListBox *LB_Adapters;
	TButton *BN_StartCapture;
	TLabel *LB_SelectedIP;
	TButton *BN_StopCapture;
	TButton *BN_Clear;
	TRichEdit *TM_Debug;
	TCheckBox *CB_FxM01Only;
	TCheckBox *CB_TimeStamp;
	TCheckBox *CB_Debug;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TMemo *TM_JUList;
	TGroupBox *GroupBox2;
	TMemo *TM_TrackList;
	TMemo *TM_MsgList;

   	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

	void __fastcall BN_LoadAdapterListClick(TObject *Sender);
	void __fastcall TB_QUITClick(TObject *Sender);
	void __fastcall LB_AdaptersCDBLlick(TObject *Sender);
    void __fastcall LB_AdaptersDblCDBLlick(TObject *Sender);
	void __fastcall BN_StartCaptureClick(TObject *Sender);
	void __fastcall BN_StopCaptureClick(TObject *Sender);
	void __fastcall BN_ClearClick(TObject *Sender);
private:	// User declarations

public:		// User declarations


	__fastcall TForm9(TComponent* Owner);

	int LoadAdapterList();

	bool isBold=false;
	TColor colour;
	void  _pme(const char* fmt);
    void  _pme(String st);

	void  pme_SetColour(TColor color, bool _isBold);
	void  pme(const char* fmt);
	void  pmeF(const char* fmt, ...);
	void  pme(String fmt);


	void __fastcall StartCapture();
	void __fastcall SelectAdapter();

	void addOrUpdateTrack(int stn, int tNum);
	void DisplayTrackList();

	void addOrUpdateJU(int stn);
	void DisplayJUList();

	void addOrUpdateJMsg(int Label, int SubLabel);
	void DisplayMsgList();


    void PrintBitString(std::vector<bool> *bitstring);

    int SelectedAdapterNumber = 0;
	_di_ITask tasks[1];

    int PlatformDPort = 1024;

    bool TaskRunning=false;

    pcap_if_t *alldevs;
	int numAdapters=0;
	pcap_t *adhandle;
	bool isLoopbackAdapter = false;
	int PacketCount=0;
	char stnbuf[50]="aa";
	int npg=0;
	int stn;


   	std::vector<bool> *bitstring;
	int curBit=0;
	int JSeriesLabel;
	int JSeriesSubLabel;

    inline int calcValue(int NumBits,std::vector<bool> *bitstring,int &curBit)
	{
		int retVal = 0;
		for (int x=0;x<NumBits;x++) retVal += (0b0001 << x) * (bitstring->at(curBit++)?1:0);
		return retVal;
	}

	void ParseJ_Series(byte * buf, int bufLen);
	void ParsePPLI(int sublabel);
	void ParseTrack(int sublabel);
	void ParseJ2_0();
	void ParseJ2_1();
	void ParseJ2_2();
    void ParseJ2_2E0();

	void ParseJ3_1();
	void ParseJ3_2();

	std::vector<String> IPAddressList;
	std::multimap<int, String> ipMap;


	std::vector<track *> JU_List;
	std::vector<track *> Track_List;
	std::vector<JMsg *> JMsgList;




};
//---------------------------------------------------------------------------





extern PACKAGE TForm9 *Form9;
//---------------------------------------------------------------------------
#endif
