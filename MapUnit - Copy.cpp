//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "MapUnit.h"
#include "GeoProj.h"

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
    //Ottawa is in  7/45/37
	VPCentreLong = OttawaLong;
	VPCentreLat = OttawaLat;
	DoDrawAll();    //callc CalcTiles
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

void  TForm1::pmeStatic(const char* fmt, ...)
{
	if (TM_Static->Lines->Count > 1000) TM_Static->Lines->Clear();
	va_list args;
	va_start(args, fmt);
	char buf[200];
	vsprintf(buf,fmt,args);
		TM_Static->Lines->Add(buf);
	va_end(args);

	//scroll to bottom of text
	TM_Static->SelStart = TM_Static->Lines->Text.Length();
	TM_Static->SelLength = 1;
	TM_Static->ClearSelection();
}



void __fastcall TForm1::DoDrawAll()
{
    CalcTiles();
	DrawMap();
	//DrawTgt();

  	TE_Zoom->Text= IntToStr(zm);
//	pme("zm:%d centreRow:%d  centreColumn:%d  x_offset: %d y_offset:%d",zm, centreRow,centreColumn,mx_offset, my_offset);
}


void  TForm1::CalcTiles()
{
   	// Get the tile containing a longitude and latitude
	Tile VPCentreTile = tile(VPCentreLong, VPCentreLat, zm);
	centreRow = VPCentreTile.y;
	centreColumn = VPCentreTile.x;
    pme(">>Centre Tile Coords z:%d x:%d  y:%d",zm,centreRow,centreColumn);

	// Get the lonlat bounding box of a tile
	LngLatBbox VPCentreTilebounds = bounds(VPCentreTile);

	double LongWidth = VPCentreTilebounds.east - VPCentreTilebounds.west;
	double deltaLong = VPCentreLong - VPCentreTilebounds.west;
	double proportionLong = deltaLong/LongWidth;
	int xFromCentre = 128 - 256*proportionLong;
	//pme("VP Centre: Lat %f  Long %f  Bounds: west: %f east: %f", VPCentreLat, VPCentreLong, VPCentreTilebounds.west,VPCentreTilebounds.east);
	//pme("Long delta: %f", deltaLong);
	//pme("VP Centre prop: %f", proportionLong);
	//pme("VP Centre: x from centre: %d",xFromCentre);

	double LatHeight = VPCentreTilebounds.north - VPCentreTilebounds.south;
	double deltaLat = VPCentreLat - VPCentreTilebounds.north;
	double proportionLat = deltaLat/LatHeight;
	int yFromCentre = 128 + 256*proportionLat;
	//pme("VP Centre y from centre: %d",yFromCentre);

	mx_offset = -xFromCentre;
	my_offset = -yFromCentre;
    pme("x offset: %d  y offset: %d",mx_offset, my_offset);
}


void  TForm1::DrawMap()
{
	TImage1->Bitmap->Canvas->BeginScene();
	TImage1->Bitmap->Canvas->Clear(0);
	TImage1->Bitmap->Canvas->EndScene();

    int tileX=0, tileY=0;
	for (int row=0;row < 4; row++) {  //these are slippy map tile indexes
		for (int column=0; column <4; column++) {//these are slippy map tile indexes
			if (mx_offset<0) tileX = row - 2;
			tileX = row - 1;
			if (my_offset<0) tileY = column - 2;
			else tileY = column - 1;
			//DrawTile(centreRow + row, centreColumn + column, zm, tileY, tileX);  //takes vp_row, vp_column and zoom
			DrawTile(centreRow + tileY, centreColumn + tileX, zm, row,column);  //takes vp_row, vp_column and zoom
		}
	}
}

//from https://learncplusplus.org/learn-about-bitmap-operations-in-c-builder-firemonkey/
//                                  these are the 3 tile indexes, then the row and column of the viewport
void __fastcall TForm1::DrawTile(int y, int x, int z,int vp_row, int vp_column)
{
	TBitmap *bmp1=new TBitmap();
	char fname[100];
	if (RB_GT->IsChecked)sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",z,x,y);
	else if (RB_ARCGIS->IsChecked) sprintf(fname,"tiles\\ARCGIS\\%d\\%d\\%d.jpg",z,x,y);
	else if (RB_OSM->IsChecked) sprintf(fname,"tiles\\osm\\%d\\%d\\%d.png",z,x,y);
	if (FileExists(fname)) bmp1->LoadFromFile(fname);
	else bmp1->LoadFromFile("tiles\\blank.png");

	int top  = vp_row * 256;  //top will be 0, 256,512, 768 etc
	int left = vp_column * 256;

	TImage1->Bitmap->Canvas->BeginScene();

		TRect Source(0,0,256,256);//basic tile is 256 x 256
		TRect Dest(left, top, left + 256,top + 256);

		if (vp_column==0) {  //left edge which may not be tile boundary
			Source.Left = mx_offset;   //Source.Right is already 256
			Dest.Left -= mx_offset;
			Dest.Right -= mx_offset;
		}
		else  //not left edge
		{
			Dest.Left -= mx_offset;
			Dest.Right -= mx_offset;
		}

		if (vp_row==0) {  //top edge which may not be tile boundary
			Source.Top = my_offset;
			Dest.Top -= my_offset;
			Dest.Bottom -= my_offset;
		}
		else  //not left edge
		{
			Dest.Top -= my_offset;
			Dest.Bottom -= my_offset;
		}

		DrawX(TImage1->Bitmap->Canvas,vp_column*256,vp_row*256,claRed);

		//outline the Dest rectangle
		TImage1->Bitmap->Canvas->DrawBitmap( bmp1, Source, Dest, 0.5, false );
		TImage1->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
		TImage1->Bitmap->Canvas->Stroke->Color = claBlue;
		TImage1->Bitmap->Canvas->Stroke->Thickness  = 1.0;
		TImage1->Bitmap->Canvas->DrawRect(Dest, 0, 0, AllCorners, 1);

		//text overlay, centere3d in Dest rectangle
		char buf[100];
		sprintf(buf,"%d/%d/%d",z,y,x);
		TImage1->Bitmap->Canvas->Fill->Color = claBlack;
		TImage1->Bitmap->Canvas->FillText(Dest, buf, false, 100, TFillTextFlags() << TFillTextFlag::RightToLeft, TTextAlign::Center,TTextAlign::Center);

	TImage1->Bitmap->Canvas->EndScene();
	bmp1->Free();
}



void __fastcall TForm1::DrawX(TCanvas *c,float x,float y,unsigned int tc)
{
	const int s = 256;
	TRectF MyRect = TRectF(x,y, x+s, y+s);
	c->Stroke->Kind = TBrushKind::Solid;
	c->Stroke->Color = tc;
	c->Stroke->Thickness  = 1.0;
	c->DrawRect(MyRect, 0, 0, AllCorners, 1);

	//TPointF myPoint = TPointF(x,y);
	//TPointF myPointe = TPointF(x+s,y+s);
	//TPointF myPoint2 = TPointF(x,y+s);
	//TPointF myPoint2e = TPointF(x+s,y);
	//c->DrawLine(myPoint,myPointe,1.0);
	//c->DrawLine(myPoint2,myPoint2e,1.0);

}






void __fastcall TForm1::TImage1MouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, bool &Handled)
{
	//pme("Wheel Delta: %d\r\n",WheelDelta);

	if (WheelDelta<0) {
		TImage1->Scale->X -= 0.1;
		TImage1->Scale->Y -= 0.1;
	}
	else
	{
		TImage1->Scale->X += 0.1;
		TImage1->Scale->Y += 0.1;
	}

	//pme("Scale: %f %f",TImage1->Scale->X,TImage1->Scale->Y);
}
//---------------------------------------------------------------------------



void __fastcall TForm1::TImage1MouseMove(TObject *Sender, TShiftState Shift, float X, float Y)
{
	//pme("Mse xy: %d %d",   (int)X, (int)Y);
	if (dragging) {
		int dx =	mouse_down_at_x - (int)X;
		int dy =	mouse_down_at_y - (int)Y;
		mx_offset = 2*dx;
		my_offset = 2*dy;
		if (dx<-256)
		{
			centreColumn-=1;
            mouse_down_at_x=(int)X;
		}
		DoDrawAll();
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImage1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
	dragging=true;
	mouse_down_at_x=(int)X;
	mouse_down_at_y=(int)Y;



	X = 2*X;  //some sort of required scaling issue
	Y = 2*Y;
	pme("Mouse Down at %f %f",X,Y);

	TImage1->Bitmap->Canvas->BeginScene();
		DrawX(TImage1->Bitmap->Canvas,X,Y,claGreen);
	TImage1->Bitmap->Canvas->EndScene();

}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImage1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  float X, float Y)
{
	pme("Mouse Up at %f %f",X,Y);
	dragging=false;
	mouse_down_at_x=0;
    mouse_down_at_y=0;

}
//---------------------------------------------------------------------------



void __fastcall TForm1::BN_QuitClick(TObject *Sender)
{
	Timer1->Enabled = false;
	Close();
}
//---------------------------------------------------------------------------


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
    pmeStatic("Hdg: %f",hdg);
}

void __fastcall TForm1::BN_PanUpClick(TObject *Sender)
{
	VPCentreLat -= .1;
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_PanDownClick(TObject *Sender)
{
	VPCentreLat += .1;
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_PanLeftClick(TObject *Sender)
{
	VPCentreLong -= .1;
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_PanRightClick(TObject *Sender)
{
    VPCentreLong += .1;
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_HomeClick(TObject *Sender)
{
	zm = 7;
	VPCentreLong = OttawaLong;
	VPCentreLat = OttawaLat;
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_ZoomInClick(TObject *Sender)
{
	++zm;
	centreRow = (2 * centreRow);
	centreColumn = (2 * centreColumn);
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BN_ZoomOutClick(TObject *Sender)
{
	--zm;
	if (zm<7) zm=7;
	else
	{
		centreRow = (int)(centreRow/2);
		centreColumn = (int)(centreColumn/2);
		DoDrawAll();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RB_GTChange(TObject *Sender)    //map type changed
{
      DoDrawAll();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SB_CentreColumnChange(TObject *Sender)
{
    //centreColumn=SB_CentreColumn->Value;
	DoDrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SB_CentreRowChange(TObject *Sender)
{
	//centreRow=SB_CentreRow->Value;
	DoDrawAll();
}
//---------------------------------------------------------------------------

int dd=0;

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	TBitmap *bmp1 = new TBitmap();
	TBitmap *bmp2 = new TBitmap();
	TBitmap *bmp3 = new TBitmap();
	TBitmap *bmp4 = new TBitmap();
	char fname[100];
	sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",7,35,46);
	if (FileExists(fname)) bmp1->LoadFromFile(fname);
	else bmp1->LoadFromFile("tiles\\blank.png");

	sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",7,36,46);
	if (FileExists(fname)) bmp2->LoadFromFile(fname);
	else bmp2->LoadFromFile("tiles\\blank.png");

	sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",7,35,47);
	if (FileExists(fname)) bmp3->LoadFromFile(fname);
	else bmp3->LoadFromFile("tiles\\blank.png");

	sprintf(fname,"tiles\\GoogleElevation\\%d\\%d\\%d.png",7,36,47);
	if (FileExists(fname)) bmp4->LoadFromFile(fname);
	else bmp4->LoadFromFile("tiles\\blank.png");


	TBitmap *master = new TBitmap(512,512);


	TRect Source(0,0,256,256);//basic tile is 256 x 256
	IM_TWO->Bitmap->Canvas->BeginScene();

		master->CopyFromBitmap(bmp1, Source, 0,0);
		master->CopyFromBitmap(bmp2, Source, 256,0);
		master->CopyFromBitmap(bmp3, Source, 0,256);
		master->CopyFromBitmap(bmp4, Source, 256,256);

		TRect Src(dd,dd,512,512);
		TRect Dest(0,0,512-dd,512-dd);
		dd+=16;

		IM_TWO->Bitmap->Canvas->DrawBitmap(master, Src, Dest, 1.0, false );

		if (false) {
			 int left =0, top=0;
			TRect Dest(left, top, left + 256,top + 256);
			IM_TWO->Bitmap->Canvas->DrawBitmap( bmp1, Source, Dest, 0.5, false );
			IM_TWO->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
			IM_TWO->Bitmap->Canvas->Stroke->Color = claBlue;
			IM_TWO->Bitmap->Canvas->Stroke->Thickness  = 1.0;
			IM_TWO->Bitmap->Canvas->DrawRect(Dest, 0, 0, AllCorners, 1);

			left+=256;
			TRect Dest2(left, top, left + 256,top + 256);
			IM_TWO->Bitmap->Canvas->DrawBitmap( bmp2, Source, Dest2, 0.5, false );
			IM_TWO->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
			IM_TWO->Bitmap->Canvas->Stroke->Color = claRed;
			IM_TWO->Bitmap->Canvas->Stroke->Thickness  = 1.0;
			IM_TWO->Bitmap->Canvas->DrawRect(Dest2, 0, 0, AllCorners, 1);

			left =0;
			top+=256;
			TRect Dest3(left, top, left + 256,top + 256);
			IM_TWO->Bitmap->Canvas->DrawBitmap( bmp3, Source, Dest3, 0.5, false );
			IM_TWO->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
			IM_TWO->Bitmap->Canvas->Stroke->Color = claGreen;
			IM_TWO->Bitmap->Canvas->Stroke->Thickness  = 1.0;
			IM_TWO->Bitmap->Canvas->DrawRect(Dest3, 0, 0, AllCorners, 1);

			left+=256;
			TRect Dest4(left, top, left + 256,top + 256);
			IM_TWO->Bitmap->Canvas->DrawBitmap( bmp4, Source, Dest4, 0.5, false );
			IM_TWO->Bitmap->Canvas->Stroke->Kind = TBrushKind::Solid;
			IM_TWO->Bitmap->Canvas->Stroke->Color = claBlack;
			IM_TWO->Bitmap->Canvas->Stroke->Thickness  = 1.0;
			IM_TWO->Bitmap->Canvas->DrawRect(Dest4, 0, 0, AllCorners, 1);
		}





	IM_TWO->Bitmap->Canvas->EndScene();
	bmp1->Free();
	bmp2->Free();
	bmp3->Free();
	bmp4->Free();
    master->Free();

	}
//---------------------------------------------------------------------------

