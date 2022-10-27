#pragma once
#ifndef Layer_h
#define Layer_h

// lớp trừu tượng cho bản đồ
class Layer
{
public:
	virtual void Render() = 0;
	virtual void Update() = 0;
};

#endif