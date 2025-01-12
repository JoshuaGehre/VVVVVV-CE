#include "Graphics.h"
#include <algorithm>

void setRect(SDL_Rect& _r, int x, int y, int w, int h)
{
	_r.x = x;
	_r.y = y;
	_r.w = w;
	_r.h = h;
}

unsigned int endian_swap(unsigned int x)
{
	return (x >> 24) |
		((x << 8) & 0x00FF0000) |
		((x >> 8) & 0x0000FF00) |
		(x << 24);
}

template <class T>
void endian_swap(T* objp)
{
	unsigned char* memp = reinterpret_cast<unsigned char*>(objp);
	std::reverse(memp, memp + sizeof(T));
}

SDL_Surface* GetSubSurface(SDL_Surface* metaSurface, int x, int y, int width, int height)
{
	// Create an SDL_Rect with the area of the _surface
	SDL_Rect area;
	area.x = x;
	area.y = y;
	area.w = width;
	area.h = height;

	// Set the RGBA mask values.
	Uint32 r, g, b, a;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	r = 0xff000000;
	g = 0x00ff0000;
	b = 0x0000ff00;
	a = 0x000000ff;
#else
	r = 0x000000ff;
	g = 0x0000ff00;
	b = 0x00ff0000;
	a = 0xff000000;
#endif

	//Convert to the correct display format after nabbing the new _surface or we will slow things down.
	SDL_Surface* preSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, r, g, b, a);
	//SDL_Surface* subSurface = SDL_DisplayFormatAlpha(preSurface);

	//SDL_FreeSurface(preSurface);

	// Lastly, apply the area from the meta _surface onto the whole of the sub _surface.
	SDL_BlitSurface(metaSurface, &area, preSurface, 0);

	// Return the new Bitmap _surface
	return preSurface;
}

void DrawPixel(SDL_Surface* _surface, int x, int y, Uint32 pixel)
{
	int bpp = _surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8* p = (Uint8*) _surface->pixels + y * _surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16*) p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER != SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32*) p = pixel;
			break;
	}
}

Uint32 ReadPixel(SDL_Surface* _surface, int x, int y)
{
	int bpp = _surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8* p = (Uint8*) _surface->pixels + y * _surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;
			break;

		case 2:
			return *(Uint16*) p;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32*) p;
			break;

		default:
			return 0; /* shouldn't happen, but avoids warnings */
	}
}

SDL_Surface* ScaleSurface(SDL_Surface* _surface, int Width, int Height, SDL_Surface* Dest)
{
	if(!_surface || !Width || !Height)
		return 0;

	SDL_Surface* _ret;
	if(Dest == NULL) {
		_ret = SDL_CreateRGBSurface(_surface->flags, Width, Height, _surface->format->BitsPerPixel,
									_surface->format->Rmask, _surface->format->Gmask, _surface->format->Bmask, _surface->format->Amask);
		if(_ret == NULL) {
			return NULL;
		}

	} else {
		_ret = Dest;
	}

	float _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(_surface->w)), _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(_surface->h));

	SDL_Rect gigantoPixel;
	for(Sint32 y = 0; y < _surface->h; y++)
		for(Sint32 x = 0; x < _surface->w; x++) {
			setRect(gigantoPixel, static_cast<Sint32>((float(x) * _stretch_factor_x) - 1), static_cast<Sint32>((float(y) * _stretch_factor_y) - 1), static_cast<Sint32>(_stretch_factor_x + 1.0), static_cast<Sint32>(_stretch_factor_y + 1.0));
			SDL_FillRect(_ret, &gigantoPixel, ReadPixel(_surface, x, y));
		}

	// DrawPixel(_ret, static_cast<Sint32>(_stretch_factor_x * x) + o_x,
	//static_cast<Sint32>(_stretch_factor_y * y) + o_y, ReadPixel(_surface, x, y));

	return _ret;
}

SDL_Surface* ScaleSurfaceSlow(SDL_Surface* _surface, int Width, int Height)
{
	if(!_surface || !Width || !Height) {
		return 0;
	}

	SDL_Surface* _ret;

	_ret = SDL_CreateRGBSurface(
		_surface->flags,
		Width,
		Height,
		_surface->format->BitsPerPixel,
		_surface->format->Rmask,
		_surface->format->Gmask,
		_surface->format->Bmask,
		_surface->format->Amask);

	if(_ret == NULL) {
		return NULL;
	}

	float _stretch_factor_x = (static_cast<double>(Width) / static_cast<double>(_surface->w)), _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(_surface->h));

	for(Sint32 y = 0; y < _surface->h; y++) {
		for(Sint32 x = 0; x < _surface->w; x++) {
			for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y) {
				for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x) {
					DrawPixel(
						_ret,
						static_cast<Sint32>(_stretch_factor_x * x) + o_x,
						static_cast<Sint32>(_stretch_factor_y * y) + o_y,
						ReadPixel(_surface, x, y));
				}
			}
		}
	}

	// DrawPixel(_ret, static_cast<Sint32>(_stretch_factor_x * x) + o_x,
	//static_cast<Sint32>(_stretch_factor_y * y) + o_y, ReadPixel(_surface, x, y));

	return _ret;
}

SDL_Surface* FlipSurfaceHorizontal(SDL_Surface* _src)
{
	SDL_Surface* ret = SDL_CreateRGBSurface(
		_src->flags,
		_src->w,
		_src->h,
		_src->format->BitsPerPixel,
		_src->format->Rmask,
		_src->format->Gmask,
		_src->format->Bmask,
		_src->format->Amask);

	if(ret == NULL) {
		return NULL;
	}

	for(Sint32 y = 0; y < _src->h; y++) {
		for(Sint32 x = 0; x < _src->w; x++) {
			DrawPixel(ret, (_src->w - 1) - x, y, ReadPixel(_src, x, y));
		}
	}

	return ret;
}

SDL_Surface* FlipSurfaceVerticle(SDL_Surface* _src)
{
	SDL_Surface* ret = SDL_CreateRGBSurface(
		_src->flags,
		_src->w,
		_src->h,
		_src->format->BitsPerPixel,
		_src->format->Rmask,
		_src->format->Gmask,
		_src->format->Bmask,
		_src->format->Amask);

	if(ret == NULL) {
		return NULL;
	}

	for(Sint32 y = 0; y < _src->h; y++) {
		for(Sint32 x = 0; x < _src->w; x++) {
			DrawPixel(ret, x, (_src->h - 1) - y, ReadPixel(_src, x, y));
		}
	}

	return ret;
}

void BlitSurfaceStandard(SDL_Surface* _src, SDL_Rect* _srcRect, SDL_Surface* _dest, SDL_Rect* _destRect)
{
	SDL_BlendMode blend = SDL_BLENDMODE_BLEND;
	SDL_GetSurfaceBlendMode(_src, &blend);
	if(blend == SDL_BLENDMODE_BLEND || blend == SDL_BLENDMODE_NONE) {
		SDL_BlitSurface(_src, _srcRect, _dest, _destRect);
	} else {
		SDL_Rect* tempRect = _destRect;

		const SDL_PixelFormat& fmt = *(_src->format);

		SDL_Surface* tempsurface = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			tempRect->w,
			tempRect->h,
			fmt.BitsPerPixel,
			fmt.Rmask,
			fmt.Gmask,
			fmt.Bmask,
			fmt.Amask);

		for(int srcX = 0; srcX < _dest->w; srcX++) {
			for(int srcY = 0; srcY < _dest->h; srcY++) {
				Uint32 src_pixel = ReadPixel(_src, srcX, srcY);

				int x = srcX + tempRect->x;
				int y = srcX + tempRect->y;
				Uint32 pixel = ReadPixel(_dest, x, y);

				Uint8 pixalpha = (pixel & _dest->format->Amask) >> _dest->format->Ashift;
				Uint8 pixred = (pixel & _dest->format->Rmask) >> _dest->format->Rshift;
				Uint8 pixgreen = (pixel & _dest->format->Gmask) >> _dest->format->Gshift;
				Uint8 pixblue = (pixel & _dest->format->Bmask) >> _dest->format->Bshift;
				Uint8 src_pixalpha = (src_pixel & _src->format->Amask) >> _src->format->Ashift;
				Uint8 src_pixred = (src_pixel & _src->format->Rmask) >> _src->format->Rshift;
				Uint8 src_pixgreen = (src_pixel & _src->format->Gmask) >> _src->format->Gshift;
				Uint8 src_pixblue = (src_pixel & _src->format->Bmask) >> _src->format->Bshift;

				if(blend == SDL_BLENDMODE_ADD) {
					pixred += std::min(double(src_pixred) * (double(src_pixalpha) / 255.0), 255.0);
					pixgreen += std::min(double(src_pixgreen) * (double(src_pixalpha) / 255.0), 255.0);
					pixblue += std::min(double(src_pixblue) * (double(src_pixalpha) / 255.0), 255.0);
				} else if(blend == SDL_BLENDMODE_MOD) {
					pixred = (double(src_pixred) / 255.0) * pixred;
					pixgreen = (double(src_pixgreen) / 255.0) * pixgreen;
					pixblue = (double(src_pixblue) / 255.0) * pixblue;
				}

				Uint32 result = (pixalpha << _dest->format->Ashift) + (pixred << _dest->format->Rshift) + (pixgreen << _dest->format->Gshift) + (pixblue << _dest->format->Bshift);

				DrawPixel(tempsurface, srcX, srcY, result);
			}
		}

		SDL_BlitSurface(tempsurface, _srcRect, _dest, tempRect);
		SDL_FreeSurface(tempsurface);
	}
}

void BlitSurfaceMasked(SDL_Surface* _src, SDL_Surface* _src2, SDL_Rect* _srcRect, SDL_Surface* _dest, SDL_Rect* _destRect, SDL_Rect* _destRect2)
{
	SDL_Rect* tempRect = _destRect;
	SDL_Rect* tempRect2 = _destRect2;
	const SDL_PixelFormat& fmt = *(_src->format);
	SDL_Surface* tempsurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		_src->w,
		_src->h,
		fmt.BitsPerPixel,
		fmt.Rmask,
		fmt.Gmask,
		fmt.Bmask,
		fmt.Amask);
	SDL_Surface* masksurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		320,
		240,
		fmt.BitsPerPixel,
		fmt.Rmask,
		fmt.Gmask,
		fmt.Bmask,
		fmt.Amask);

	SDL_BlitSurface(_src2, _srcRect, masksurface, tempRect2);

	for(int x = 0; x < tempsurface->w; x++) {
		for(int y = 0; y < tempsurface->h; y++) {
			Uint32 pixel = ReadPixel(_src, x, y);
			Uint32 Alpha = pixel & fmt.Amask;
			Uint32 CTAlpha = 255;
			if(((x + _destRect->x >= 0) && (x + _destRect->x < 320)) && ((y + _destRect->y >= 0) && (y + _destRect->y < 240))) {
				Uint32 pixel2 = ReadPixel(masksurface, x + _destRect->x, y + _destRect->y);
				CTAlpha = pixel2 & fmt.Amask;
			}

			Uint32 result = pixel & 0x00FFFFFF;
			float div1 = ((Alpha >> 24) / 255.0f);
			float div2 = ((CTAlpha >> 24) / 255.0f);
			Uint32 UseAlpha = (div1 * div2) * 255.0f;
			DrawPixel(tempsurface, x, y, result | (UseAlpha << 24));
		}
	}

	SDL_BlitSurface(tempsurface, _srcRect, _dest, tempRect);
	SDL_FreeSurface(tempsurface);
}

void BlitSurfaceColoured(
	SDL_Surface* _src,
	SDL_Rect* _srcRect,
	SDL_Surface* _dest,
	SDL_Rect* _destRect,
	colourTransform& ct,
	int flipped /*= 0*/
)
{
	if(ct.nocolor) return BlitSurfaceStandard(_src, _srcRect, _dest, _destRect);

	SDL_Rect* tempRect = _destRect;

	const SDL_PixelFormat& fmt = *(_src->format);
	// const SDL_PixelFormat& destfmt = *(_dest->format);

	SDL_Surface* tempsurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		_src->w,
		_src->h,
		fmt.BitsPerPixel,
		fmt.Rmask,
		fmt.Gmask,
		fmt.Bmask,
		fmt.Amask);

	for(int x = 0; x < tempsurface->w; x++) {
		for(int y = 0; y < tempsurface->h; y++) {
			int srcx = x;
			if(flipped) {
				srcx = flipped - x - 1;
				if(srcx < 0) continue;
			}
			Uint32 pixel = ReadPixel(_src, srcx, y);
			Uint32 Alpha = pixel & fmt.Amask;
			Uint32 result = ct.colour & 0x00FFFFFF;
			Uint32 CTAlpha = ct.colour & fmt.Amask;
			float div1 = ((Alpha >> 24) / 255.0f);
			float div2 = ((CTAlpha >> 24) / 255.0f);
			Uint32 UseAlpha = (div1 * div2) * 255.0f;
			DrawPixel(tempsurface, x, y, result | (UseAlpha << 24));
		}
	}

	SDL_BlitSurface(tempsurface, _srcRect, _dest, tempRect);
	SDL_FreeSurface(tempsurface);
}

void EraseSurface(
	SDL_Surface* _src,
	SDL_Surface* _dest)
{
	for(int x = 0; x < _dest->w; x++) {
		for(int y = 0; y < _dest->h; y++) {
			Uint32 src_pixel = ReadPixel(_src, x, y);
			Uint32 pixel = ReadPixel(_dest, x, y);

			Uint8 pixred = (pixel & _dest->format->Rmask) >> 16;
			Uint8 pixgreen = (pixel & _dest->format->Gmask) >> 8;
			Uint8 pixblue = (pixel & _dest->format->Bmask) >> 0;
			Uint8 pixalpha = (src_pixel & _src->format->Amask) == 0 ? (pixel & _dest->format->Amask) : 0;

			Uint32 result = pixalpha | (pixred << 16) | (pixgreen << 8) | (pixblue << 0);

			DrawPixel(_dest, x, y, result);
		}
	}
}

void BlitSurfaceTinted(
	SDL_Surface* _src,
	SDL_Rect* _srcRect,
	SDL_Surface* _dest,
	SDL_Rect* _destRect,
	colourTransform& ct)
{
	SDL_Rect* tempRect = _destRect;

	const SDL_PixelFormat& fmt = *(_src->format);

	SDL_Surface* tempsurface = SDL_CreateRGBSurface(
		SDL_SWSURFACE,
		_src->w,
		_src->h,
		fmt.BitsPerPixel,
		fmt.Rmask,
		fmt.Gmask,
		fmt.Bmask,
		fmt.Amask);

	for(int x = 0; x < tempsurface->w; x++) {
		for(int y = 0; y < tempsurface->h; y++) {
			Uint32 pixel = ReadPixel(_src, x, y);

			Uint8 pixred = (pixel & _src->format->Rmask) >> 16;
			Uint8 pixgreen = (pixel & _src->format->Gmask) >> 8;
			Uint8 pixblue = (pixel & _src->format->Bmask) >> 0;

			double temp_pixred = pixred * 0.299;
			double temp_pixgreen = pixgreen * 0.587;
			double temp_pixblue = pixblue * 0.114;

			double gray = floor((temp_pixred + temp_pixgreen + temp_pixblue + 0.5));

			Uint8 ctred = (ct.colour & graphics.backBuffer->format->Rmask) >> 16;
			Uint8 ctgreen = (ct.colour & graphics.backBuffer->format->Gmask) >> 8;
			Uint8 ctblue = (ct.colour & graphics.backBuffer->format->Bmask) >> 0;

			temp_pixred = gray * ctred / 255.0;
			temp_pixgreen = gray * ctgreen / 255.0;
			temp_pixblue = gray * ctblue / 255.0;

			if(temp_pixred > 255)
				temp_pixred = 255;
			if(temp_pixgreen > 255)
				temp_pixgreen = 255;
			if(temp_pixblue > 255)
				temp_pixblue = 255;

			pixred = temp_pixred;
			pixgreen = temp_pixgreen;
			pixblue = temp_pixblue;

			Uint32 Alpha = pixel & fmt.Amask;
			Uint32 result = (pixred << 16) + (pixgreen << 8) + (pixblue << 0);
			Uint32 CTAlpha = ct.colour & fmt.Amask;
			float div1 = ((Alpha >> 24) / 255.0f);
			float div2 = ((CTAlpha >> 24) / 255.0f);
			Uint32 UseAlpha = (div1 * div2) * 255.0f;

			DrawPixel(tempsurface, x, y, result | (UseAlpha << 24));
		}
	}

	SDL_BlitSurface(tempsurface, _srcRect, _dest, tempRect);
	SDL_FreeSurface(tempsurface);
}

int scrollamount = 0;
bool isscrolling = 0;

void UpdateFilter()
{
	if(rand() % 4000 < 8) {
		isscrolling = true;
	}

	if(isscrolling == true) {
		scrollamount += 20;
		if(scrollamount > 240) {
			scrollamount = 0;
			isscrolling = false;
		}
	}
}

SDL_Surface* ApplyFilter(SDL_Surface* _src)
{
	SDL_Surface* _ret = SDL_CreateRGBSurface(_src->flags, _src->w, _src->h, 32,
											 _src->format->Rmask, _src->format->Gmask, _src->format->Bmask, _src->format->Amask);

	int redOffset = rand() % 4;

	for(int x = 0; x < _src->w; x++) {
		for(int y = 0; y < _src->h; y++) {
			int sampley = (y + scrollamount) % 240;

			Uint32 pixel = ReadPixel(_src, x, sampley);

			Uint8 green = (pixel & _src->format->Gmask) >> 8;
			Uint8 blue = (pixel & _src->format->Bmask) >> 0;

			Uint32 pixelOffset = ReadPixel(_src, std::min(x + redOffset, 319), sampley);
			Uint8 red = (pixelOffset & _src->format->Rmask) >> 16;

			if(isscrolling && sampley > 220 && ((rand() % 10) < 4)) {
				red = std::min(int(red + (fRandom() * 0.6) * 254), 255);
				green = std::min(int(green + (fRandom() * 0.6) * 254), 255);
				blue = std::min(int(blue + (fRandom() * 0.6) * 254), 255);
			} else {
				red = std::min(int(red + (fRandom() * 0.2) * 254), 255);
				green = std::min(int(green + (fRandom() * 0.2) * 254), 255);
				blue = std::min(int(blue + (fRandom() * 0.2) * 254), 255);
			}

			if(y % 2 == 0) {
				red = static_cast<Uint8>(red / 1.2f);
				green = static_cast<Uint8>(green / 1.2f);
				blue = static_cast<Uint8>(blue / 1.2f);
			}

			int distX = static_cast<int>((abs(160.0f - x) / 160.0f) * 16);
			int distY = static_cast<int>((abs(120.0f - y) / 120.0f) * 32);

			red = std::max(red - (distX + distY), 0);
			green = std::max(green - (distX + distY), 0);
			blue = std::max(blue - (distX + distY), 0);

			Uint32 finalPixel = ((red << 16) + (green << 8) + (blue << 0)) | (pixel & _src->format->Amask);
			DrawPixel(_ret, x, y, finalPixel);
		}
	}
	return _ret;
}

SDL_Surface* ApplyCuteFilter(SDL_Surface* _src)
{
	SDL_Surface* _ret = SDL_CreateRGBSurface(_src->flags, _src->w, _src->h, 32,
											 _src->format->Rmask, _src->format->Gmask, _src->format->Bmask, _src->format->Amask);

	for(int x = 0; x < _src->w; x++) {
		for(int y = 0; y < _src->h; y++) {
			Uint32 pixel = ReadPixel(_src, x, y);
			Uint8 red = (pixel & _src->format->Rmask) >> 16;
			Uint8 green = (pixel & _src->format->Gmask) >> 8;
			Uint8 blue = (pixel & _src->format->Bmask) >> 0;

			double temp_red = red;
			double temp_green = green;
			double temp_blue = blue;

			// We have to grayscale it

			temp_red = temp_red * 0.299;
			temp_green = temp_green * 0.587;
			temp_blue = temp_blue * 0.114;

			double gray = floor((temp_red + temp_green + temp_blue + 0.5));

			temp_red = gray * 1.4;
			temp_green = gray / 1.5;
			temp_blue = gray * 1.4;

			if(temp_red > 255) temp_red = 255;
			if(temp_green > 255) temp_green = 255;
			if(temp_blue > 255) temp_blue = 255;

			red = temp_red;
			green = temp_green;
			blue = temp_blue;

			Uint32 finalPixel = ((red << 16) + (green << 8) + (blue << 0)) | (pixel & _src->format->Amask);
			DrawPixel(_ret, x, y, finalPixel);
		}
	}
	return _ret;
}

SDL_Surface* ApplyAllyFilter(SDL_Surface* _src)
{
	SDL_Surface* _ret = SDL_CreateRGBSurface(_src->flags, _src->w, _src->h, 32,
											 _src->format->Rmask, _src->format->Gmask, _src->format->Bmask, _src->format->Amask);

	int band = _src->h / 5;

	for(int x = 0; x < _src->w; x++) {
		for(int y = 0; y < _src->h; y++) {
			Uint32 pixel = ReadPixel(_src, x, y);
			Uint8 red = (pixel & _src->format->Rmask) >> 16;
			Uint8 green = (pixel & _src->format->Gmask) >> 8;
			Uint8 blue = (pixel & _src->format->Bmask) >> 0;

			double temp_red = red;
			double temp_green = green;
			double temp_blue = blue;

			// We have to grayscale it

			temp_red = temp_red * 0.299;
			temp_green = temp_green * 0.587;
			temp_blue = temp_blue * 0.114;

			double gray = floor((temp_red + temp_green + temp_blue + 0.5));

			if(y >= 0 && y < band) { // Orange
				temp_red = gray * 1.4;
				temp_green = gray / 1.3;
				temp_blue = gray / 1.5;
			} else if(y >= band && y < band * 2) { // Lighter orange
				temp_red = gray * 1.4;
				temp_green = gray / 1.15;
				temp_blue = gray / 1.5;
			} else if(y >= band * 2 && y < band * 3) { // White
				temp_red = gray * 1.1;
				temp_green = gray / 1.1;
				temp_blue = gray * 1.1;
			} else if(y >= band * 3 && y < band * 4) { // Pink
				temp_red = gray * 1.4;
				temp_green = gray / 1.3;
				temp_blue = gray * 1.2;
			} else if(y >= band * 4 && y < band * 5) { // Darker pink
				temp_red = gray * 1.2;
				temp_green = gray / 1.3;
				temp_blue = gray * 1;
			}

			if(temp_red > 255) temp_red = 255;
			if(temp_green > 255) temp_green = 255;
			if(temp_blue > 255) temp_blue = 255;

			red = temp_red;
			green = temp_green;
			blue = temp_blue;

			Uint32 finalPixel = ((red << 16) + (green << 8) + (blue << 0)) | (pixel & _src->format->Amask);
			DrawPixel(_ret, x, y, finalPixel);
		}
	}
	return _ret;
}

SDL_Surface* ApplyMisaFilter(SDL_Surface* _src)
{
	SDL_Surface* _ret = SDL_CreateRGBSurface(_src->flags, _src->w, _src->h, 32,
											 _src->format->Rmask, _src->format->Gmask, _src->format->Bmask, _src->format->Amask);

	int band = _src->h / 5;

	for(int x = 0; x < _src->w; x++) {
		for(int y = 0; y < _src->h; y++) {
			Uint32 pixel = ReadPixel(_src, x, y);
			Uint8 red = (pixel & _src->format->Rmask) >> 16;
			Uint8 green = (pixel & _src->format->Gmask) >> 8;
			Uint8 blue = (pixel & _src->format->Bmask) >> 0;

			double temp_red = red;
			double temp_green = green;
			double temp_blue = blue;

			// We have to grayscale it

			temp_red = temp_red * 0.299;
			temp_green = temp_green * 0.587;
			temp_blue = temp_blue * 0.114;

			double gray = floor((temp_red + temp_green + temp_blue + 0.5));

			if((y >= 0 && y < band) || y >= band * 4) {
				temp_red = gray / 1.5;
				temp_green = gray * 0.9;
				temp_blue = gray * 1.3;
			} else if((y >= band && y < band * 2) || (y >= band * 3 && y < band * 4)) {
				temp_red = gray * 1.4;
				temp_green = gray / 1.5;
				temp_blue = gray * 1.4;
			} else {
				temp_red = gray * 1.1;
				temp_green = gray * 1.1;
				temp_blue = gray * 1.1;
			}

			if(temp_red > 255) temp_red = 255;
			if(temp_green > 255) temp_green = 255;
			if(temp_blue > 255) temp_blue = 255;

			red = temp_red;
			green = temp_green;
			blue = temp_blue;

			Uint32 finalPixel = ((red << 16) + (green << 8) + (blue << 0)) | (pixel & _src->format->Amask);
			DrawPixel(_ret, x, y, finalPixel);
		}
	}
	return _ret;
}

void FillRect(SDL_Surface* _surface, const int _x, const int _y, const int _w, const int _h, const int r, int g, int b)
{
	SDL_Rect rect = {Sint16(_x), Sint16(_y), Sint16(_w), Sint16(_h)};
	Uint32 color;
	color = SDL_MapRGB(_surface->format, r, g, b);
	SDL_FillRect(_surface, &rect, color);
}

void FillRect(SDL_Surface* _surface, const int r, int g, int b)
{
	SDL_Rect rect = {0, 0, Uint16(_surface->w), Uint16(_surface->h)};
	Uint32 color;
	color = SDL_MapRGB(_surface->format, r, g, b);
	SDL_FillRect(_surface, &rect, color);
}

void FillRect(SDL_Surface* _surface, const int color)
{
	SDL_Rect rect = {0, 0, Uint16(_surface->w), Uint16(_surface->h)};
	SDL_FillRect(_surface, &rect, color);
}

void FillRect(SDL_Surface* _surface, const int x, const int y, const int w, const int h, int rgba)
{
	SDL_Rect rect = {Sint16(x), Sint16(y), Sint16(w), Sint16(h)};
	SDL_FillRect(_surface, &rect, rgba);
}

void FillRect(SDL_Surface* _surface, SDL_Rect& _rect, const int r, int g, int b)
{
	Uint32 color;
	color = SDL_MapRGB(_surface->format, r, g, b);
	SDL_FillRect(_surface, &_rect, color);
}

void FillRect(SDL_Surface* _surface, SDL_Rect rect, int rgba)
{
	SDL_FillRect(_surface, &rect, rgba);
}

bool intersectRect(float left1, float right1, float bottom1, float top1, float left2, float right2, float bottom2, float top2)
{
	return !(left2 > right1 || right2 < left1 || top2 < bottom1 || bottom2 > top1);
}

void OverlaySurfaceKeyed(SDL_Surface* _src, SDL_Surface* _dest, Uint32 _key)
{
	// const SDL_PixelFormat& fmt = *(_src->format);
	for(int x = 0; x < _src->w; x++) {
		for(int y = 0; y < _src->h; y++) {
			Uint32 pixel = ReadPixel(_src, x, y);
			//endian_swap(pixel);
			if((pixel != _key)) {
				DrawPixel(_dest, x, y, pixel);
			}
		}
	}
}

void ScrollSurface(SDL_Surface* _src, int _pX, int _pY)
{
	SDL_Surface* part1 = NULL;

	SDL_Rect rect1;
	SDL_Rect rect2;
	//scrolling up;
	if(_pY < 0) {
		setRect(rect2, 0, 0, _src->w, _src->h - _pY);

		part1 = GetSubSurface(_src, rect2.x, rect2.y, rect2.w, rect2.h);

		SDL_Rect destrect1;

		SDL_SetSurfaceBlendMode(part1, SDL_BLENDMODE_NONE);

		setRect(destrect1, 0, _pY, _pX, _src->h);

		SDL_BlitSurface(part1, NULL, _src, &destrect1);
	}

	else if(_pY > 0) {

		setRect(rect1, 0, 0, _src->w, _src->h - _pY);

		part1 = GetSubSurface(_src, rect1.x, rect1.y, rect1.w, rect1.h);

		SDL_Rect destrect1;

		SDL_SetSurfaceBlendMode(part1, SDL_BLENDMODE_NONE);

		setRect(destrect1, _pX, _pY, _src->w, _src->h - _pY);

		SDL_BlitSurface(part1, NULL, _src, &destrect1);

	}

	//Right
	else if(_pX <= 0) {
		setRect(rect2, 0, 0, _src->w - _pX, _src->h);

		part1 = GetSubSurface(_src, rect2.x, rect2.y, rect2.w, rect2.h);

		SDL_Rect destrect1;

		SDL_SetSurfaceBlendMode(part1, SDL_BLENDMODE_NONE);

		setRect(destrect1, _pX, 0, _src->w - _pX, _src->h);

		SDL_BlitSurface(part1, NULL, _src, &destrect1);
	}

	else if(_pX > 0) {

		setRect(rect1, _pX, 0, _src->w - _pX, _src->h);

		part1 = GetSubSurface(_src, rect1.x, rect1.y, rect1.w, rect1.h);

		SDL_Rect destrect1;

		SDL_SetSurfaceBlendMode(part1, SDL_BLENDMODE_NONE);

		setRect(destrect1, 0, 0, _src->w - _pX, _src->h);

		SDL_BlitSurface(part1, NULL, _src, &destrect1);
	}
	//Cleanup temp surface
	if(part1) {
		SDL_FreeSurface(part1);
	}
}

RgbColor HsvToRgb(HsvColor hsv)
{
	RgbColor rgb;
	unsigned char region, remainder, p, q, t;

	if(hsv.s == 0) {
		rgb.r = hsv.v;
		rgb.g = hsv.v;
		rgb.b = hsv.v;
		return rgb;
	}

	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;

	p = (hsv.v * (255 - hsv.s)) >> 8;
	q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

	switch(region) {
		case 0:
			rgb.r = hsv.v;
			rgb.g = t;
			rgb.b = p;
			break;
		case 1:
			rgb.r = q;
			rgb.g = hsv.v;
			rgb.b = p;
			break;
		case 2:
			rgb.r = p;
			rgb.g = hsv.v;
			rgb.b = t;
			break;
		case 3:
			rgb.r = p;
			rgb.g = q;
			rgb.b = hsv.v;
			break;
		case 4:
			rgb.r = t;
			rgb.g = p;
			rgb.b = hsv.v;
			break;
		default:
			rgb.r = hsv.v;
			rgb.g = p;
			rgb.b = q;
			break;
	}

	return rgb;
}

HsvColor RgbToHsv(RgbColor rgb)
{
	HsvColor hsv;
	unsigned char rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if(hsv.v == 0) {
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if(hsv.s == 0) {
		hsv.h = 0;
		return hsv;
	}

	if(rgbMax == rgb.r)
		hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if(rgbMax == rgb.g)
		hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return hsv;
}
