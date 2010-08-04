#ifndef __SgColor_h_
#define __SgColor_h_

class SgColor {
public:
  void * operator new(size_t size);
  
  void operator delete( void * obj );
  
  static SgColor* FindColor(float h, float s, float v,
			    const char* schemename = NULL);
  static SgColor* FindColor(short r, short g, short b,
			    const char* schemename = NULL);
  static void RGBtoHSV(short r, short g, short b, 
		       float &h, float &s, float &v);
  static void HSVtoRGB(float h, float s, float v, 
		       short &r, short &g, short &b);
  ~SgColor();
  
  short getRed() {return red;}
  short getGreen() {return green;}
  short getBlue() {return blue;}
  void getHSV(float& h, float& s, float& v);
  
  
  char* getString();		// Get a string representing this color.
  
protected:
  SgColor(short r, short g, short b, const char* schemename);
  
  short red;
  short green;
  short blue;
  char* schemename;
  
  SgColor* next;
  static SgColor* first;
};

#endif /* __SgColor_h_ */

