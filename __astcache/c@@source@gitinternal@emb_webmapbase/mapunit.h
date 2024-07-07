//---------------------------------------------------------------------------

#ifndef MapUnitH
#define MapUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.Edit.hpp>
#include <FMX.EditBox.hpp>
#include <FMX.NumberBox.hpp>
#include <FMX.SpinBox.hpp>

#include "MercatorTile.h"
#include "TileFunctions.h"
using namespace mercatortile;

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMemo *MemoDebug;
	TBitmapAnimation *BitmapAnimation1;
	TImage *TImage1;
	TButton *BN_Quit;
	TTimer *Timer1;
	TButton *BN_StartStop;
	TButton *BN_Home;
	TButton *BN_ZoomIn;
	TButton *BN_ZoomOut;
	TRadioButton *RB_GT;
	TRadioButton *RB_ARCGIS;
	TRadioButton *RB_OSM;
	TPanel *Panel1;
	TCheckBox *CB_Debug;
	TButton *BN_Clear;
	TGroupBox *GP_MapControl;
	TCheckBox *CB_TileOutlines;
	TCheckBox *CB_TileCoords;
	TCheckBox *CB_BigX;
	void __fastcall TImage1MouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
		  bool &Handled);
	void __fastcall TImage1MouseMove(TObject *Sender, TShiftState Shift, float X, float Y);
	void __fastcall TImage1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
	void __fastcall TImage1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y);
	void __fastcall BN_QuitClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall BN_StartStopClick(TObject *Sender);
	void __fastcall BN_HomeClick(TObject *Sender);
	void __fastcall RB_GTChange(TObject *Sender);
	void __fastcall BN_ClearClick(TObject *Sender);
	void __fastcall Panel1Exit(TObject *Sender);
	void __fastcall CB_TileOutlinesChange(TObject *Sender);



private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void  pme(const char* fmt, ...);
	void  pmeStatic(const char* fmt, ...);
	void __fastcall DoDrawAll();
    void  BuildAndDrawMap();
	void __fastcall DrawTgt();
    void __fastcall DrawBox(TCanvas *c,float x,float y,float hdg,unsigned int tc);

	//void __fastcall DrawTile(int y, int x, int z,int location);
	void __fastcall DrawTile(int y, int x, int z,int vert, int hori);

	void  pmeTileInfo(Tile t);
    void  Calc3by3andCentreTile();

	TBitmap  *GetTile(int z,int x,int m);
    void __fastcall ClearMapCache();


	bool b_dragging=false;

	double ProjLeft;
	double ProjTop;
	double ProjRight;
	double ProjBottom;
	double ProjWidth;
	double ProjHeight;

	int ViewPortWidth=512;
	int ViewPortHeight=512;
	double rHoriPerPixel;
	double rVertPerPixel;


	float hdg=15;
	int centreRow = 45;
	int centreColumn = 37;
	const int _centreRow =45;
	const int _centreColumn = 37;

    double OttawaLat = 45.400;
	double OttawaLong = -75.690;


	int zm = 7;
	int mx_offset;
	int my_offset;
	double VPCentreLat = 45.400;
	double VPCentreLong = -75.690;




	bool dragging=false;
	int mouse_down_at_x=0;
	int mouse_down_at_y=0;

	int g_Left;
	int g_Top;

	TBitmap *master;
    bool MasterLoaded = false;

    std::vector<TileCoord> loadedTileList;


};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
