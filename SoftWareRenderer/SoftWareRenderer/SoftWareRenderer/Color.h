#pragma once
//color r:0-255 g:0-255 b:0-255
struct  Color
{
public:
	Color(){ _r = _b = _g = 0; };
	Color(byte r, byte g, byte b) :_r(r), _g(g), _b(b){};
	friend Color operator * (Color a, Color b)
	{//modulate color
		byte R = byte((a.GetR() / 255.0f) * (b.GetR() / 255.0f) * 255);
		byte G = byte((a.GetG() / 255.0f) * (b.GetG() / 255.0f) * 255);
		byte B = byte((a.GetB() / 255.0f) * (b.GetB() / 255.0f) * 255);
		return Color(R, G, B);
	};
	friend Color operator * (float a, Color b)
	{
		float R = a * b.GetR();
		float G = a * b.GetG();
		float B = a * b.GetB();
		return Color(Color::LimitRange(R), Color::LimitRange(G), Color::LimitRange(B));
	};
	friend Color operator * (Color a, float b)
	{
		float R = b * a.GetR();
		float G = b * a.GetG();
		float B = b * a.GetB();
		return Color(Color::LimitRange(R), Color::LimitRange(G), Color::LimitRange(B));
	};
	friend Color operator + (Color a, Color b)
	{
		int R = a.GetR() + b.GetR();
		int G = a.GetG() + b.GetG();
		int B = a.GetB() + b.GetB();
		return Color(Color::LimitRange(R), Color::LimitRange(G), Color::LimitRange(B));
	};

	byte GetR(){ return _r; };
	byte GetB(){ return _b; };
	byte GetG(){ return _g; };

	void SetR(byte r){ _r = Color::LimitRange(r); };
	void SetB(byte b){ _b = Color::LimitRange(b); };
	void SetG(byte g){ _g = Color::LimitRange(g); };

	static byte LimitRange(int v)
	{
		if (v <= 0)
		{
			return 0;
		}
		if (v >= 255)
		{
			return 255;
		}
		return v;
	};

	static byte LimitRange(float v)
	{
		if (v <= 0)
		{
			return 0;
		}
		if (v >= 255)
		{
			return 255;
		}
		return (byte)v;
	}

private:
	 byte _r;
	 byte _b;
	 byte _g;
};