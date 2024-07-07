#ifndef TILECOORDH
#define TILECOORDH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>



struct TileCoord {
public:
	int z; // zoom    [0......20]
	int x; // x index [0...z^2-1]
	int y; // y index [0...z^2-1]

	TBitmap * tile;
	bool loaded;

};


#endif