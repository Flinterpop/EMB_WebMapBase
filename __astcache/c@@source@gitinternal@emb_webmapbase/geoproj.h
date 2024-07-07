#pragma once



double lonToX(double lon);
double latToY(double lat);
double xToLon(double x);
double yToLat(double y);

//double RadToDeg(double rad);
//double DegToRad(double deg);



int long2tilex(double lon, int z);
int lat2tiley(double lat, int z);
double tilex2long(int x, int z);
double tiley2lat(int y, int z);

