#include "Transform.h"

void Transform::SetX(float x) { X = x; } // thay đổi vị trí x
void Transform::SetY(float y) { Y = y; } // thay đổi vị trí y
float Transform::GetX() { return X; } // lấy tọa độ x
float Transform::GetY() { return Y; } // lấy tọa độ y

void Transform::Log(string mag) // in trạng thái
{
	cout << mag << "(X;Y) = (" << X << ";" << Y << ")" << endl;
}