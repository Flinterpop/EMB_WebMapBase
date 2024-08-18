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
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
	NoTile = new TBitmap(TILESIZE, TILESIZE);
	NoTile->Canvas->BeginScene();
	NoTile->Clear(claSilver);
	NoTile->Canvas->Fill->Color = claBlack;
	NoTile->Canvas->FillText(TRect(0,0,256,128), "No Tile", false, 100, TFillTextFlags() << TFillTextFlag::RightToLeft, TTextAlign::Center,TTextAlign::Center);
	NoTile->Canvas->EndScene();

	DoViewResetToHome();
	DoDrawAll();

}
//---------------------------------------------------------------------------



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

void  TForm1::pmeMouse(const char* fmt, ...)
{
	if (TM_Mouse->Lines->Count > 1000) TM_Mouse->Lines->Clear();

	va_list args;
	va_start(args, fmt);
	char buf[200];
	vsprintf(buf,fmt,args);
		TM_Mouse->Lines->Add(buf);
	va_end(args);

	//scroll to bottom of text
	TM_Mouse->SelStart = TM_Mouse->Lines->Text.Length();
	TM_Mouse->SelLength = 1;
	TM_Mouse->ClearSelection();
}



void __fastcall TForm1::DoDrawAll()
{
    CalcCenterTileAndOffsetOf_POIfromCentreOfItsItsTile();
	Calc3x3UnshiftedProjection();
	CalcViewPortShiftedProjection();
	Build5x5BitMap();

	DrawMap();
	DrawNotes();
	DrawTgt();


	// Convert  web mercator x, y to longtitude and latitude.
	LngLat TL = lnglat(m_ViewPortBBox.left, m_ViewPortBBox.top);
	LngLat BR = lnglat(m_ViewPortBBox.right, m_ViewPortBBox.bottom);

    TM_Mouse->Lines->Clear();
	pmeMouse("Corners:");
	pmeMouse("Top Left: %5.2f %6.2f:",TL.lat ,TL.lng);
   	pmeMouse("Bottom Right: %5.2f %6.2f:",BR.lat ,BR.lng);

}


//Calculates 3 member variables: mx_offset, my_offset,  m_ViewPortCentreTile in x,y,z
//based on POI that should be in centre of map
//only needs to be called once and then whenever the map pans or zooms
void __fastcall TForm1::CalcCenterTileAndOffsetOf_POIfromCentreOfItsItsTile()
{
	//save the centre Row and Column before updating
	int lastCentreRow = m_ViewPortCentreTile.y;
	int lastCentreColumn = m_ViewPortCentreTile.x;

	// Get the tile containing a longitude and latitude of the current centre
	m_ViewPortCentreTile = tile(VPCentreLL.lng, VPCentreLL.lat, zm);
	//pme(">>Centre Tile Coords z:%d x:%d  y:%d", ViewPortCentreTile.z, ViewPortCentreTile.x, ViewPortCentreTile.y);

	//if the centre tile has changed then force downstream rebuild of the master 5 x 5 bitmap
	if ( (lastCentreRow != m_ViewPortCentreTile.y) || (lastCentreColumn != m_ViewPortCentreTile.x) )
	{
		if (MasterBitmapLoaded) m_BitMap5x5->Free();
		MasterBitmapLoaded = false;
	}

	// Get the Proj bounding box of the centre tile
	Bbox ViewPortCentreTileboundsXY = xy_bounds(m_ViewPortCentreTile);
	//pme("CalcOS:ViewPortCentreTilebounds top:%5.0f bottom:%5.0f  left:%5.0f  right:%5.0f", ViewPortCentreTileboundsXY.top,ViewPortCentreTileboundsXY.bottom, 		ViewPortCentreTileboundsXY.left,ViewPortCentreTileboundsXY.right);

    // Calculate Projection Coords of centre feature
	XY POI_Pxy = xy(VPCentreLL.lng, VPCentreLL.lat);

	//calc vertical offset in pixels
	double TileHeightInY = ViewPortCentreTileboundsXY.top - ViewPortCentreTileboundsXY.bottom;
	double distanceFromTopEdgeInY = ViewPortCentreTileboundsXY.top - POI_Pxy.y;  //from POI to edge
	double proportionY = distanceFromTopEdgeInY/TileHeightInY;
	int yFromTopInPixels = TILESIZE * proportionY;
	my_offset = yFromTopInPixels - HALF_TILESIZE;
	//pme("CalcOS:POI deltaY = %f   POI my_offset: %d   POI yFromTopInPixels:%d",distanceFromTopEdgeInY, my_offset,yFromTopInPixels);

	//calc horizontal offset in pixels
	//double TileWidthInX = ViewPortCentreTileboundsXY.right - ViewPortCentreTileboundsXY.left;
	//double distanceFromLeftEdgeInX = POI_Pxy.x - ViewPortCentreTileboundsXY.left;  //from POI to edge
	//double proportionX = distanceFromLeftEdgeInX/TileWidthInX;
	//int xFromLeftInPixels = TILESIZE * proportionX;
	//mx_offset = xFromLeftInPixels - HALF_TILESIZE;


	double TileWidthInX = ViewPortCentreTileboundsXY.left - ViewPortCentreTileboundsXY.right;
	double distanceFromLeftEdgeInX = ViewPortCentreTileboundsXY.left - POI_Pxy.x;  //from POI to edge
	double proportionX = distanceFromLeftEdgeInX/TileWidthInX;
	int xFromLeftInPixels = TILESIZE * proportionX;
	mx_offset = xFromLeftInPixels - HALF_TILESIZE;


    //pme("mx_offset: %d  my_offset: %d",mx_offset,my_offset);


	//pme("CalcOS:POI deltaX = %f   POI mx_offset: %d   POI xFromLeftInPixels:%d",distanceFromLeftEdgeInX, mx_offset,xFromLeftInPixels);
}


//calculates 3x3 bounding box based on centre tile x,y,z
//calculates width and height of 3x3 in projection coords
//calculates Projection Units per pixel in 2D
//only needs to be called once and then whenever the centre tile changes
void  __fastcall TForm1::Calc3x3UnshiftedProjection()
{
	//struct Tile TopLeft and BottomRight
   	struct Tile TL3x3, BR3x3;
		TL3x3.x = m_ViewPortCentreTile.x - 1;
		TL3x3.y = m_ViewPortCentreTile.y - 1;
		TL3x3.z= zm;

		BR3x3.x = m_ViewPortCentreTile.x + 1;
		BR3x3.y = m_ViewPortCentreTile.y + 1;
		BR3x3.z= zm;

	// Get the Proj Bounding box of the unshifted 3x3
	m_3x3BBox = xy_bounds(TL3x3);   // Get the web mercator bounding box of a tile  //only top and left are needed at this point

	Bbox BR_XY = xy_bounds(BR3x3);    // Get the web mercator bounding box of a tile
	m_3x3BBox.right = BR_XY.right;
	m_3x3BBox.bottom = BR_XY.bottom;

	m_X_3x3Width  = (m_3x3BBox.left - m_3x3BBox.right);
	m_Y_3x3Height = (m_3x3BBox.top  - m_3x3BBox.bottom);

	m_ProjPerHoriPixel = m_X_3x3Width / (TILESIZE*3);
	m_ProjPerVertPixel = m_Y_3x3Height / (TILESIZE*3);

	//pmeMouse("Widths: Y: %5.0f  X: %5.01f", m_Y_3x3Height,m_X_3x3Width);
	//pmeMouse("Non-Shifted:BB top: %5.0f  bottom: %5.0f  left: %5.0f   right: %5.0f",m_3x3BBox.top,m_3x3BBox.bottom,m_3x3BBox.left,m_3x3BBox.right);
}


//calculates Viewport Bounding box
// used by LLtoVPposition()
//used by MouseToLatLong()
void __fastcall TForm1::CalcViewPortShiftedProjection()
{
	//offsets are in mouse pixels
	double HorizontalOffsetInProjection = (double)(mx_offset)*m_ProjPerHoriPixel;
	double VPLeftProj = m_3x3BBox.left - HorizontalOffsetInProjection;

	double VerticalOffsetInProjection = (double)(my_offset)*m_ProjPerVertPixel;
	double VPTopProj = m_3x3BBox.top - VerticalOffsetInProjection;


	double VPRightProj  = VPLeftProj -  m_X_3x3Width;
	double VPBottomProj = VPTopProj  -  m_Y_3x3Height;

	m_ViewPortBBox.left  = VPLeftProj;
	m_ViewPortBBox.top   = VPTopProj;
	m_ViewPortBBox.right = VPRightProj;
	m_ViewPortBBox.bottom = VPBottomProj;

	//pmeMouse("Shifted:BB top: %5.0f  bottom: %5.0f  left: %5.0f   right: %5.0f",m_ViewPortBBox.top,m_ViewPortBBox.bottom,m_ViewPortBBox.left,m_ViewPortBBox.right);

}


//builds the m_BitMap5x5
//only called once or when the map pans beyond a tile
void  TForm1::Build5x5BitMap()
{
	if (MasterBitmapLoaded == true) return;
	//only build the 5 x 5 master bitmap if its not already in memory
		MasterBitmapLoaded = true;
		m_BitMap5x5 = new TBitmap(TILESIZE * 5,TILESIZE * 5);

		m_BitMap5x5->Canvas->BeginScene();
			//build a 5 x 5 bitmap
			for (int row=0;row < 5; row++) {  //these are slippy map tile indexes
				for (int column=0; column <5; column++) {//these are slippy map tile indexes

					int x = m_ViewPortCentreTile.x + column - 2;
					int y = m_ViewPortCentreTile.y + row - 2;

					TBitmap *bmp1 = GetTileBitmap(zm,x,y);
					m_BitMap5x5->CopyFromBitmap(bmp1, TRect(0,0,TILESIZE,TILESIZE), column * TILESIZE,row*TILESIZE);

					//bmp1->Free(); ////To DO - this causes a crash. Why? it shouldn't be needed after its copied

					int r[] = {0,TILESIZE,2*TILESIZE,3*TILESIZE,4*TILESIZE};
					TRect Dest( r[column], r[row], TILESIZE + r[column], TILESIZE+r[row]);

					if (CB_TileOutlines->IsChecked) {
						//draw outline around each tile
						m_BitMap5x5->Canvas->Stroke->Kind = TBrushKind::Solid;
						m_BitMap5x5->Canvas->Stroke->Color = claBlue;
						m_BitMap5x5->Canvas->Stroke->Thickness  = 1.0;
						m_BitMap5x5->Canvas->DrawRect(Dest, 0, 0, AllCorners, 1.0);
					}
					if (CB_TileCoords->IsChecked) {
						//text overlay, centere3d in Dest rectangle
						char buf[100];
						sprintf(buf,"%d/%d/%d",zm,y,x);
						m_BitMap5x5->Canvas->Fill->Color = claBlack;
						m_BitMap5x5->Canvas->FillText(Dest, buf, false, 100, TFillTextFlags() << TFillTextFlag::RightToLeft, TTextAlign::Center,TTextAlign::Center);
					}
				}
			}
		m_BitMap5x5->Canvas->EndScene();
}

TBitmap  *TForm1::GetTileBitmap(int z,int x,int y)
{
	for (TileCoord t : loadedTileList) {
		if ((t.z==z) && (t.x==x) && (t.y==y)) return t.tile;
	}

    //to get here the tile is not loaded into memory, so go get it from disk
	char fname[140];
	if (RB_GT->IsChecked)sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",zm,x,y);
	else if (RB_ARCGIS->IsChecked) sprintf(fname,"tiles\\ARCGIS\\%d\\%d\\%d.jpg",zm,x,y);
	else if (RB_OSM->IsChecked) sprintf(fname,"tiles\\osm\\%d\\%d\\%d.png",zm,x,y);

	TileCoord t;
	t.z=z; t.x=x; t.y=y;

	TBitmap *bmp1;
	if (FileExists(fname))
	{
		bmp1=new TBitmap();
		bmp1->LoadFromFile(fname);
		t.loaded=true;
	}
	else
	{
		bmp1 = NoTile;
        t.loaded=false;
	}

	t.tile = bmp1;
	loadedTileList.push_back(t);
	return bmp1;
}


//draws the background map base layer
//draws a 3 tile by 3 tile bitmap from within an enclosing 5 by 5 tile bitmap shifted by POI offset
void  TForm1::DrawMap()
{
    //g_Left g_Top are the upper corner (in pixels) of the 3 x 3 tile map to be displayed on screen when the 3x3 is shifted by the offset from centre POI
	g_Left = TILESIZE + mx_offset;
	g_Top  = TILESIZE + my_offset;

    //draw the shifted master bitmap
	TImage1->Bitmap->Canvas->BeginScene();
		TImage1->Bitmap->Canvas->Clear(0);
		TRect SourceRect(g_Left, g_Top, g_Left + (TILESIZE*3), g_Top + (TILESIZE*3));
		TImage1->Bitmap->Canvas->DrawBitmap(m_BitMap5x5, SourceRect, TRect(0,0,(TILESIZE*3),(TILESIZE*3)), 0.7, false ); //2nd last param is opacity

		if (CB_BigX->IsChecked) {
			//Draw X through entire map
			TImage1->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
			TImage1->Bitmap->Canvas->Stroke->Color = claRed;
			TImage1->Bitmap->Canvas->Stroke->Thickness  = 1.0;
			TImage1->Bitmap->Canvas->DrawLine(TPointF(0,0),TPointF((TILESIZE*3), (TILESIZE*3)),1.0);
			TImage1->Bitmap->Canvas->DrawLine(TPointF((TILESIZE*3), 0),TPointF(0, (TILESIZE*3)),1.0);
		}
	TImage1->Bitmap->Canvas->EndScene();
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
	LngLat LL = ViewPortXYToLatLong(X,Y);

	if (CB_Debug->IsChecked) pmeMouse(">>MM: Mse xy: %f %f\r\n%5.2f %6.2f", X, Y, LL.lat, LL.lng);

	XY mxy = xy(LL.lng, LL.lat);
	sprintf(mouseOverlayText,"%d %d\r\n%5.4f %6.4f\r\n%5.0f  %5.0f", (int)X, (int)Y, LL.lat, LL.lng,mxy.x,mxy.y);

	if (dragging) {
		int dx =	mouse_down_at_x - (int)X;
		int dy =	mouse_down_at_y - (int)Y;
		mouse_down_at_x = X;
		mouse_down_at_y = Y;

		double deltaPX = m_ProjPerHoriPixel*dx;
		double deltaPY = m_ProjPerVertPixel*dy;

		// Convert longtitude and latitude to web mercator x, y.
		XY vpXY = xy(VPCentreLL.lng, VPCentreLL.lat);
		vpXY.x -= deltaPX;
		vpXY.y -= deltaPY;

		// update the centre tile LL
		VPCentreLL = lnglat(vpXY.x, vpXY.y);
	}

	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImage1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
	//TCursor Save_Cursor = TForm1->
	if (Button ==  TMouseButton::mbLeft)
	{
		dragging=true;
		//Panel1->Cursor = crHandPoint;
	}
	else if (Button ==  TMouseButton::mbRight)
	{
		LngLat LL = ViewPortXYToLatLong(X,Y);
		HomeLong = LL.lng;
		HomeLat = LL.lat;
        DoViewResetToHome();

    }

	mouse_down_at_x=(int)X;
	mouse_down_at_y=(int)Y;
	if (CB_Debug->IsChecked) pmeMouse("Mouse Down at %f %f",X,Y);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImage1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y)
{
	dragging = false;
	//Panel1->Cursor = crDefault;
	mouse_down_at_x = 0;
	mouse_down_at_y = 0;
	if (CB_Debug->IsChecked) pmeMouse("Mouse Up at %f %f",X,Y);

}
//---------------------------------------------------------------------------



void __fastcall TForm1::BN_QuitClick(TObject *Sender)
{
	Timer1->Enabled = false;
	ClearMapCache();
	NoTile->Free();
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::DoViewResetToHome()
{
	//zm = 7;
	VPCentreLL.lng = HomeLong;
	VPCentreLL.lat = HomeLat;
	//TImage1->Scale->X=1.35;
	//TImage1->Scale->Y=1.35;
	ClearMapCache();
	DoDrawAll();
}


void __fastcall TForm1::BN_HomeClick(TObject *Sender)
{
	DoViewResetToHome();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClearMapCache()
{
	for (auto t : loadedTileList)
	{
		if (t.loaded) t.tile->Free();
	}
	loadedTileList.clear();
	if (MasterBitmapLoaded) m_BitMap5x5->Free();
  	MasterBitmapLoaded = false;
}

void __fastcall TForm1::RefreshMap(TObject *Sender)    //map type changed
//one of the map service provider Radio Buttons was changed
{
	ClearMapCache();
	DoDrawAll();
}
//---------------------------------------------------------------------------


static double DEG2RAD = M_PI / 180.0;

void __fastcall TForm1::DrawTrack(TCanvas *c,float x,float y,float hdg,unsigned int tc)
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
	//pme("Hdg: %f",hdg);
}

void __fastcall TForm1::DrawRect(TCanvas *c,TRect MyRect, unsigned int tc)
{
	c->Stroke->Kind = TBrushKind::Solid;
	c->Stroke->Color = tc;
	c->Stroke->Thickness  = 1.0;
	c->DrawRect(MyRect, 0, 0, AllCorners, 1);
}

void __fastcall TForm1::FillRect(TCanvas *c,TRect MyRect, unsigned int tc)
{
	//c->Stroke->Kind = TBrushKind::Solid;
	c->Fill->Color = tc;
	//c->Stroke->Thickness  = 1.0;
	c->FillRect(MyRect, 0, 0, AllCorners, 1);
}


double tlat = 45.0;
double tlon = -75.0;
double hdg = 45.0;
int tnum=0;

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	//DoDrawAll();

	DrawMap();
	DrawNotes();

	tnum=0;
	DrawTgt();
	++tnum;   	DrawTgt();
	++tnum;   	DrawTgt();
	++tnum;   	DrawTgt();
	++tnum;   	DrawTgt();
	++tnum;   	DrawTgt();
	++tnum;   	DrawTgt();


	tlat-=.001;
	tlon+=.001;
	hdg+=10;
	if (hdg>=360) hdg=0;

}
//---------------------------------------------------------------------------


void __fastcall TForm1::DrawTgt()
{
	XY mxy = LLtoVPposition(tlat,tlon);

    mxy.x +=tnum*5;
	TImage1->Bitmap->Canvas->BeginScene();
	DrawTrack(TImage1->Bitmap->Canvas,mxy.x,mxy.y,hdg,claBlack);
	//DrawTrack(TImage1->Bitmap->Canvas,mxy.y,mxy.x,hdg,claBlack);
	TImage1->Bitmap->Canvas->EndScene();
}



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


void __fastcall TForm1::DrawNotes()
{

	TRect Dest( 10, 10, TILESIZE, TILESIZE/2);
	TImage1->Bitmap->Canvas->BeginScene();
	TImage1->Bitmap->Canvas->Fill->Color = claCrimson;
	DrawRect(TImage1->Bitmap->Canvas,Dest,claWhite);
	FillRect(TImage1->Bitmap->Canvas,Dest,claBlack);

	TImage1->Bitmap->Canvas->Font->Size = 20.0;

	TImage1->Bitmap->Canvas->Fill->Color = claWhite;

	TImage1->Bitmap->Canvas->FillText(Dest, mouseOverlayText, true, 100, TFillTextFlags(), TTextAlign::Center,TTextAlign::Center);
	TImage1->Bitmap->Canvas->EndScene();

}



void __fastcall TForm1::BN_ClearClick(TObject *Sender)
{
	MemoDebug->Lines->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1Exit(TObject *Sender)
{
	dragging=false;
}
//---------------------------------------------------------------------------




void __fastcall TForm1::BN_ZeroClick(TObject *Sender)
{
	mx_offset=0;
	my_offset=0;
		ClearMapCache();
	DoDrawAll();
}
//---------------------------------------------------------------------------



XY __fastcall TForm1::LLtoVPposition(double lat, double lng)
{
	XY thisXY = xy(lng, lat);  //get the Mercator Projection
	//pme("--------------------------");
	//pme("=>Tgt: %5.2f  %6.2f",lat,lng);
	//pme("=>Proj: x:%5.0f y: %5.0f",thisXY.x, thisXY.y);


	//double deltaXFromLeft = thisXY.x - m_ViewPortBBox.left ;
	double deltaXFromLeft = m_ViewPortBBox.left - thisXY.x;

	double x = (deltaXFromLeft / m_ProjPerHoriPixel);


	double deltaYFromTop =  m_ViewPortBBox.top - thisXY.y;
	double y = (deltaYFromTop / m_ProjPerVertPixel);

	//pme("***VPort: left: %5.0f  top: %5.0f",m_ViewPortBBox.left, m_ViewPortBBox.top);
	//pme("***dXfromLeft: %5.0f  dyFromTop: %5.0f",deltaXFromLeft, deltaYFromTop);
	//pme("***x: %5.0f  y: %5.0f",x, y);


	thisXY.x = x;
	thisXY.y = y;
	return thisXY;

}

LngLat TForm1::ViewPortXYToLatLong(float x, float y)
{
	double scale;// = 2.0;
	int IH = TImage1->Height;
	int IW = TImage1->Width;
	scale = 768.0/((double)IH);
	//pme("IHeight: %d  IWidth: %d  Scale is %f",IH,IW,scale);

	double PX = m_ViewPortBBox.left - (m_ProjPerHoriPixel * x * scale);
	double PY = m_ViewPortBBox.top - (m_ProjPerVertPixel * y * scale);

	LngLat LL = lnglat(PX, PY);

	//pme("mse xy: %2.0f %2.0f",x,y);
	//pme("mse Px: %5.0f  mse Py: %5.0f",PX,PY);
	//pmeMouse("Mouse: PY:%5.0f PX:%5.0f  Lat:%5.1f  Lng: %5.1f",PY,PX,LL.lat,LL.lng);

	return LL;
}



void __fastcall TForm1::BN_TestClick(TObject *Sender)
{
	if (1.5==	TImage1->Scale->X) {
		TImage1->Scale->X = 1.0;
		TImage1->Scale->Y = 1.0;
	}
	else
	{
		TImage1->Scale->X = 1.5;
		TImage1->Scale->Y = 1.5;
	}


	pme("Scale: %f",TImage1->MultiResBitmap->Items[0]->Scale);

    //TImage1->MultiResBitmap->Items[0]->Scale =0.9;




}
//---------------------------------------------------------------------------

