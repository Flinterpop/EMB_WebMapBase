//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MapUnit.h"
#include "GeoProj.h"
#include <vector>

//from https://github.com/sshuair/MercatorTile/tree/master?tab=readme-ov-file#utility-functions
#include "MercatorTile.h"
using namespace mercatortile;


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
    //Figure out the scaling/////////////////
	TImage1->Scale->X=1.35;
    TImage1->Scale->Y=1.35;

	VPCentreLong = OttawaLong;
	VPCentreLat = OttawaLat;
	DoDrawAll();
}
//---------------------------------------------------------------------------


void  TForm1::pmeTileInfo(Tile stile)
{
	Bbox rbbox = xy_bounds(stile);
	pme("the tile {%d, %d, %d} xy bounds is\r\n left:%f, bottom:%f, right:%f, top:%f ", stile.x, stile.y, stile.z, rbbox.left, rbbox.bottom, rbbox.right, rbbox.top);

	LngLatBbox llbbox = bounds(stile);
	pme("the tile {%d, %d, %d} LL bounds is\r\n west:%f, south:%f, east:%f, north:%f ", stile.x, stile.y, stile.z, llbbox.west, llbbox.south, llbbox.east, llbbox.north);
}

void  TForm1::pme(const char* fmt, ...)
{
	if (MemoDebug->Lines->Count > 1000) MemoDebug->Lines->Clear();

	va_list args;
	va_start(args, fmt);
	char buf[200];
	vsprintf(buf,fmt,args);
		MemoDebug->Lines->Add(buf);
	va_end(args);

	//scroll to bottom of text
	MemoDebug->SelStart = MemoDebug->Lines->Text.Length();
	MemoDebug->SelLength = 1;
	MemoDebug->ClearSelection();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::DoDrawAll()
{
    Calc3by3andCentreTile();
	BuildAndDrawMap();
	//DrawTgt();
	//	pme("zm:%d centreRow:%d  centreColumn:%d  x_offset: %d y_offset:%d",zm, centreRow,centreColumn,mx_offset, my_offset);
}


void  TForm1::Calc3by3andCentreTile()
{
   	// Get the tile containing a longitude and latitude
	Tile VPCentreTile = tile(VPCentreLong, VPCentreLat, zm);
	int lastCentreRow = centreRow;
	int lastCentreColumn = centreColumn;
	centreRow = VPCentreTile.y;
	centreColumn = VPCentreTile.x;

    //if the centre tile has changed then rebuild the master
	if ( (lastCentreRow != centreRow) || (lastCentreColumn != centreColumn) )
	{
		if (MasterLoaded) master->Free();
        MasterLoaded = false;
	}
	if (CB_Debug->IsChecked) pme(">>Centre Tile Coords z:%d x:%d  y:%d",zm,centreRow,centreColumn);

	// Get the lonlat bounding box of a tile
	LngLatBbox VPCentreTilebounds = bounds(VPCentreTile);

	double LongWidth = VPCentreTilebounds.east - VPCentreTilebounds.west;
	double deltaLong = VPCentreLong - VPCentreTilebounds.west;
	double proportionLong = deltaLong/LongWidth;
	int xFromCentre = 128 - 256*proportionLong;

	double LatHeight = VPCentreTilebounds.north - VPCentreTilebounds.south;
	double deltaLat = VPCentreLat - VPCentreTilebounds.north;
	double proportionLat = deltaLat/LatHeight;
	int yFromCentre = 128 + 256 * proportionLat;

	mx_offset = -xFromCentre;
	my_offset = -yFromCentre;

    //g_Left g_Top are the upper corner of the 3 x 3 tile map to be displayed on screen
	g_Left = (640 + mx_offset) - 384;    //640 is half of 1280 = 5 *256
	g_Top  = (640 + my_offset) - 384;

	if (CB_Debug->IsChecked) {
		pme("VP Centre: Lat %f  Long %f  Bounds: west: %f east: %f", VPCentreLat, VPCentreLong, VPCentreTilebounds.west,VPCentreTilebounds.east);
		pme("Long delta: %f", deltaLong);
		pme("VP Centre prop: %f", proportionLong);
		pme("VP Centre: x from centre: %d",xFromCentre);

		pme("VP Centre y from centre: %d",yFromCentre);
		pme("x offset: %d  y offset: %d",mx_offset, my_offset);
		pme("g_Left: %d   g_Top: %d",g_Left, g_Top);
	}

}


TBitmap  *TForm1::GetTile(int z,int x,int y)
{
	for (TileCoord t : loadedTileList) {
		if ((t.z==z) && (t.x==x) && (t.y==y)) return t.tile;
	}

    //to get here the tile is not loaded into memory, so go get it from disk
	char fname[140];
	if (RB_GT->IsChecked)sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",zm,x,y);
	else if (RB_ARCGIS->IsChecked) sprintf(fname,"tiles\\ARCGIS\\%d\\%d\\%d.jpg",zm,x,y);
	else if (RB_OSM->IsChecked) sprintf(fname,"tiles\\osm\\%d\\%d\\%d.png",zm,x,y);

	TBitmap *bmp1=new TBitmap();
	if (FileExists(fname)) bmp1->LoadFromFile(fname);
	else bmp1->LoadFromFile("tiles\\blank.png"); // not guarded should use an in memory bitmap//do this later

	TileCoord t;
    t.z=z; t.x=x; t.y=y; t.tile = bmp1;
	loadedTileList.push_back(t);
    return bmp1;
}

void  TForm1::BuildAndDrawMap()
{
	if (MasterLoaded == false) {//only build the 5 x 5 master bitmap if its not the same already in memory
		MasterLoaded = true;
		master = new TBitmap(256*5,256*5);

		master->Canvas->BeginScene();
			//build a 5 x 5 bitmap
			for (int row=0;row < 5; row++) {  //these are slippy map tile indexes
				for (int column=0; column <5; column++) {//these are slippy map tile indexes

					int x = centreColumn + column - 2;
					int y = centreRow + row - 2;

					TBitmap *bmp1 = GetTile(zm,x,y);
					master->CopyFromBitmap(bmp1, TRect(0,0,256,256), column * 256,row*256);

					int r[] = {0,256,512,768,1024};
					TRect Dest( r[column], r[row], 256 + r[column], 256+r[row]);
					if (CB_TileOutlines->IsChecked) {
						//draw outline around each tile
						master->Canvas->Stroke->Kind = TBrushKind::Solid;
						master->Canvas->Stroke->Color = claBlue;
						master->Canvas->Stroke->Thickness  = 1.0;
						master->Canvas->DrawRect(Dest, 0, 0, AllCorners, 1.0);
					}
					if (CB_TileCoords->IsChecked) {
						//text overlay, centere3d in Dest rectangle
						char buf[100];
						sprintf(buf,"%d/%d/%d",zm,y,x);
						master->Canvas->Fill->Color = claBlack;
						master->Canvas->FillText(Dest, buf, false, 100, TFillTextFlags() << TFillTextFlag::RightToLeft, TTextAlign::Center,TTextAlign::Center);
					}

				}
			}
		master->Canvas->EndScene();
	}


    //draw the shifted master bitmap
	TImage1->Bitmap->Canvas->BeginScene();
		TImage1->Bitmap->Canvas->Clear(0);
		TRect Source(g_Left, g_Top ,g_Left + 768, g_Top + 768);
		TImage1->Bitmap->Canvas->DrawBitmap(master, Source, TRect(0,0,768,768), 0.75, false );

		if (CB_BigX->IsChecked) {
			//Draw X through entire map
			TImage1->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
			TImage1->Bitmap->Canvas->Stroke->Color = claRed;
			TImage1->Bitmap->Canvas->Stroke->Thickness  = 1.0;
			TImage1->Bitmap->Canvas->DrawLine(TPointF(0,0),TPointF(768, 768),1.0);
			TImage1->Bitmap->Canvas->DrawLine(TPointF(768, 0),TPointF(0, 768),1.0);
		}
	TImage1->Bitmap->Canvas->EndScene();

    //master->Free();
}

//from https://learncplusplus.org/learn-about-bitmap-operations-in-c-builder-firemonkey/



void __fastcall TForm1::TImage1MouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, bool &Handled)
{
	if (Shift.Contains(ssShift)) {
		if (WheelDelta<0) {
			TImage1->Scale->X -= 0.05;
			TImage1->Scale->Y -= 0.05;
		}
		else
		{
			TImage1->Scale->X += 0.05;
			TImage1->Scale->Y += 0.05;
		}
	}
	else
	{
		if (WheelDelta<0) {
			if (--zm<1) zm = 1;
			else DoDrawAll();
		}
		else
		{
			if (++zm>20) zm = 20;
			else DoDrawAll();
		}

	}


}
//---------------------------------------------------------------------------



void __fastcall TForm1::TImage1MouseMove(TObject *Sender, TShiftState Shift, float X, float Y)
{
	if (CB_Debug->IsChecked) pme("Mse xy: %d %d",   (int)X, (int)Y);

	if (dragging) {
		int dx =	mouse_down_at_x - (int)X;
		int dy =	mouse_down_at_y - (int)Y;
		mouse_down_at_x=X;
		mouse_down_at_y=Y;
		//                  0  1    2    3     4     5     6     7     8      9       10      11     12     13      14
		float panRates[] = {0, 0.1, 0.2, 0.07, 0.05, 0.02, 0.02, 0.01, 0.005, 0.0025, 0.0025, 0.001, 0.001, 0.0005, 0.0004  };
        float dd=0;
		if (zm>14) dd = .0001;
		else dd = panRates[zm];
        //pme("dd: %1.4f",dd);

		VPCentreLong = VPCentreLong + dx * dd;
		VPCentreLat = VPCentreLat  - dy * dd;
		DoDrawAll();
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImage1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
	//TCursor Save_Cursor = TForm1->
	if (Button ==  TMouseButton::mbLeft)
	{
		dragging=true;
        Panel1->Cursor = crHandPoint;
	}
	mouse_down_at_x=(int)X;
	mouse_down_at_y=(int)Y;
	if (CB_Debug->IsChecked) pme("Mouse Down at %f %f",X,Y);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImage1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y)
{
	dragging=false;
	Panel1->Cursor = crDefault;
	mouse_down_at_x=0;
	mouse_down_at_y=0;
	if (CB_Debug->IsChecked) pme("Mouse Up at %f %f",X,Y);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::BN_QuitClick(TObject *Sender)
{
	Timer1->Enabled = false;
	Close();
}
//---------------------------------------------------------------------------




void __fastcall TForm1::BN_HomeClick(TObject *Sender)
{
	zm = 7;
	VPCentreLong = OttawaLong;
	VPCentreLat = OttawaLat;
   	TImage1->Scale->X=1.35;
	TImage1->Scale->Y=1.35;
	ClearMapCache();
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClearMapCache()
{
	  loadedTileList.clear();
	  if (MasterLoaded) master->Free();
	  MasterLoaded=false;
}

void __fastcall TForm1::RB_GTChange(TObject *Sender)    //map type changed
{
	ClearMapCache();
	DoDrawAll();
}
//---------------------------------------------------------------------------


static double DEG2RAD = M_PI / 180.0;

void __fastcall TForm1::DrawBox(TCanvas *c,float x,float y,float hdg,unsigned int tc)
{
	const int s = 16;
	const int hs = s/2;

	TRectF MyRect = TRectF(x-hs,y-hs, x+hs, y+hs);
	c->Stroke->Kind = TBrushKind::Solid;
	c->Stroke->Color = tc;
	c->Stroke->Thickness  = 1.0;
	c->DrawRect(MyRect, 0, 0, AllCorners, 1);

	float x2 = x + s*sin(DEG2RAD*(hdg));
	float y2 = y - s*cos(DEG2RAD*(hdg));

	TPointF myPoint = TPointF(x,y);
	TPointF myPoint2 = TPointF(x2,y2);
	c->DrawLine(myPoint,myPoint2,1.0);
	c->DrawLine(myPoint2,myPoint2,1.0);
    pme("Hdg: %f",hdg);
}




void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_StartStopClick(TObject *Sender)
{
	if (Timer1->Enabled==false) {
		Timer1->Enabled=true;
		BN_StartStop->Text="STOP";
	}
	else
	{
		Timer1->Enabled=false;
		BN_StartStop->Text="START";
	}
}


int dx=0;

void __fastcall TForm1::DrawTgt()
{
	double tlat = 45.099;
	double tlon = -75.9667;

	XY txy = xy(tlon,tlat);

	int mx = -(txy.x - ProjLeft)/rHoriPerPixel;
	int my = -(txy.y - ProjTop)/rVertPerPixel;


	mx+=dx;
	my+=dx;
	dx+=4;

	mx=2*mx; //scaling problem to be researched
	my = 2*my;
	pme("tgt loc: %d %d",mx,my);



	TImage1->Bitmap->Canvas->BeginScene();
		DrawBox(TImage1->Bitmap->Canvas,mx,my,hdg,claBlack);
	TImage1->Bitmap->Canvas->EndScene();

	int w = TImage1->Width;
	int h = TImage1->Height;
	pme("Width: %d  Height: %d",w,h);
	hdg+=15;
	if (hdg>=360) hdg=0;

}

void __fastcall TForm1::BN_ClearClick(TObject *Sender)
{
    MemoDebug->Lines->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1Exit(TObject *Sender)
{
	dragging=false;
	Panel1->Cursor = crDefault;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CB_TileOutlinesChange(TObject *Sender)
{
	ClearMapCache();
    DoDrawAll();
}
//---------------------------------------------------------------------------

