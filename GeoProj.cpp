#include <iostream>
#include <cmath>

#include "GeoProj.h"

using namespace std;

//  MercatorProjection EPSG:3395

    //https://wiki.openstreetmap.org/wiki/Mercator#JavaScript
    //The above page provides web mercator and true mercator code. This file uses the true code:
    //Elliptical (true) Mercator Projection
    //This projection gives more accurate aspect ratios for objects anywhere on Earth,
    //and respects their angles with higher precision.
    //However, this projection is not commonly used on most maps used on the OSM websites and in editors.

    //https://spatialreference.org/ref/epsg/wgs-84-world-mercator/
    //EPSG:3395
    //WGS 84 / World Mercator(Google it)
    //WGS84 Bounds: -180.0000, -80.0000, 180.0000, 84.0000
    //Projected Bounds: -20037508.3428, -15496570.7397, 20037508.3428, 18764656.2314
    //Scope: Very small scale mapping.
    //Last Revised: June 2, 2006
    //Area: World - between 80°S and 84°N

    //from: https://gis.stackexchange.com/questions/259121/transformation-functions-for-epsg3395-projection-vs-epsg3857
    //You have discovered the reason for the following information extracted from an NGA Advisory Notice on "Web Mercator".
    //"The NGA Geomatics Office has assessed the use of Web Mercator and other non-WGS 84 spatial reference systems may cause geo-location / geo-coordinate errors up to 40,000 meters. This erroneous geospatial positioning information poses an unacceptable risk to global safety of navigation activities, and department of defense, intelligence community, and allied partner systems, missions, and operations that require accurate and precise positioning and navigation information. The NGA Geomatics Office reminds the community to use DoD approved World Geodetic System 1984 (WGS 84) applications for all mission critical activities."
    //EPSG:3395 is WGS 84 compliant.EPSG:3857 is NOT! You are seeing the reason why.




# define M_PI           3.14159265358979323846  /* pi */

static double R_MAJOR = 6378137.0;
static double R_MINOR = 6356752.3142;
static double RATIO = R_MINOR / R_MAJOR;
static double ECCENT = sqrt(1.0 - (RATIO * RATIO));
static double COM = 0.5 * ECCENT;

static double DEG2RAD = M_PI / 180.0;
static double RAD2Deg = 180.0 / M_PI;
static double PI_2 = M_PI / 2.0;


double lonToX(double lon)
{
    return R_MAJOR * DegToRad(lon);
}

double xToLon(double x)
{
    return RadToDeg(x) / R_MAJOR;
}


double latToY(double lat)
{
    lat = fmin(89.5, fmax(lat, -89.5));
    //lat = Math.Min(89.5, Math.Max(lat, -89.5));

    double phi = DegToRad(lat);
    double sinphi = sin(phi);
    double con = ECCENT * sinphi;
    //con = Math.Pow(((1.0 - con) / (1.0 + con)), COM);
    con = pow(((1.0 - con) / (1.0 + con)), COM);

    double ts = tan(0.5 * ((M_PI * 0.5) - phi)) / con;
    double retVal = 0 - R_MAJOR * log(ts);
    if (abs(retVal) < 1e-9) retVal = 0.0; //added by Brad G
    return retVal;
}


double yToLat(double y)
{
    double ts = exp(-y / R_MAJOR);
    double phi = PI_2 - 2 * atan(ts);
    double dphi = 1.0;
    int i = 0;
    while ((abs(dphi) > 0.000000001) && (i < 15))
    {
        double con = ECCENT * sin(phi);
        dphi = PI_2 - 2 * atan(ts * pow((1.0 - con) / (1.0 + con), COM)) - phi;
        phi += dphi;
        i++;
    }
    return RadToDeg(phi);
}



/*
double RadToDeg(double rad)
{
	return rad * RAD2Deg;
}

double DegToRad(double deg)
{
	return deg * DEG2RAD;
}
  */



int long2tilex(double lon, int z)
{
	return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
}

int lat2tiley(double lat, int z)
{
    double latrad = lat * M_PI/180.0;
	return (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z)));
}

double tilex2long(int x, int z)
{
	return x / (double)(1 << z) * 360.0 - 180;
}

double tiley2lat(int y, int z)
{
	double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}



