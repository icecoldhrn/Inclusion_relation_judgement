#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

//全局判断精度
#define EPSILON 0.000001

using namespace std;

//定义二维double点矢量
struct  point
{
	double x, y;

	point()
	{
		x = 0.0;
		y = 0.0;
	}
	point(double dx, double dy)
	{
		x = dx;
		y = dy;
	}
	void Printpoint()
	{
		cout << "（" << x << "，" << y << "）" << endl;
	}
};

//判断点在线段上，包括端点
bool IsPointOnLine(double pointx, double pointy, double linex1, double liney1, double linex2, double liney2)
{
	bool flag = false;
	double d = (linex1 - pointx) * (liney2 - pointy) - (linex2 - pointx) * (liney1 - pointy);
	if ((abs(d) < EPSILON) && ((pointx - linex1) * (pointx - linex2) <= 0) && ((pointy - liney1) * (pointy - liney2) <= 0))
	{
		flag = true;
	}
	return flag;
}

//判断两线段相交,包括邻接
bool Is2LineIntersect(double linex1, double liney1, double linex2, double liney2, double linex3, double liney3, double linex4, double liney4)
{
	bool flag = false;
	double d = (linex2 - linex1) * (liney4 - liney3) - (liney2 - liney1) * (linex4 - linex3);
	if (d != 0)
	{
		double r = ((liney1 - liney3) * (linex4 - linex3) - (linex1 - linex3) * (liney4 - liney3)) / d;
		double s = ((liney1 - liney3) * (linex2 - linex1) - (linex1 - linex3) * (liney2 - liney1)) / d;
		if ((r >= 0) && (r <= 1) && (s >= 0) && (s <= 1))
		{
			flag = true;
		}
	}
	return flag;
}

//判断线段1包含线段2
bool IsLine1InLine2(double linex1, double liney1, double linex2, double liney2, double linex3, double liney3, double linex4, double liney4)
{
	bool flag1 = IsPointOnLine(linex3, liney3, linex1, liney1, linex2, liney2);
	bool flag2 = IsPointOnLine(linex4, liney4, linex1, liney1, linex2, liney2);
	bool flag = (flag1 && flag2);

	return flag;
}

//判断点在多边形内部,不包括在多边形的边上
bool IsPointInPolygon(double x, double y, const vector<point>& polygon)
{
	bool flag = false;
	int count = 0;

	//寻找最小X
	double minX = DBL_MAX;
	for (unsigned int i = 0; i < polygon.size(); i++)
	{
		minX = std::min(minX, polygon[i].x);
	}


	double px = x;
	double py = y;
	double linePoint1x = x;
	double linePoint1y = y;
	double linePoint2x = minX - 1;			//取最小的X值再小的值作为射线的终点
	double linePoint2y = y;

	//遍历每一条边
	for (unsigned int i = 0; i < polygon.size() - 1; i++)
	{
		double polx1 = polygon[i].x;
		double poly1 = polygon[i].y;
		double polx2 = polygon[i + 1].x;
		double poly2 = polygon[i + 1].y;

		if (IsPointOnLine(px, py, polx1, poly1, polx2, poly2))  //排除点在边上，即邻接情况
		{
			return false;
		}

		if (fabs(poly2 - poly1) < EPSILON)   //射线与边线平行则不相交
		{
			continue;
		}

		if (IsPointOnLine(polx1, poly1, linePoint1x, linePoint1y, linePoint2x, linePoint2y))
		{
			if (poly1 > poly2)			//只保证上端点+1
			{
				count++;
			}
		}
		else if (IsPointOnLine(polx2, poly2, linePoint1x, linePoint1y, linePoint2x, linePoint2y))
		{
			if (poly2 > poly1)			//只保证上端点+1
			{
				count++;
			}
		}
		else if (Is2LineIntersect(polx1, poly1, polx2, poly2, linePoint1x, linePoint1y, linePoint2x, linePoint2y))   //已经排除平行的情况
		{
			count++;
		}
	}

	if (count % 2 == 1)        //射线法。交点个数为奇数，则点在多边形内部
	{
		flag = true;
	}

	return flag;
}

//判断线段在多边形内，不包括邻接情况
bool IsLineInPolygon(double linex1, double liney1, double linex2, double liney2, const vector<point>& polygon)
{
	bool flag, flag1, flag2;


	for (unsigned int i = 0; i < polygon.size() - 1; i++)
	{
		double polx1 = polygon[i].x;
		double poly1 = polygon[i].y;
		double polx2 = polygon[i + 1].x;
		double poly2 = polygon[i + 1].y;
		if (Is2LineIntersect(linex1, liney1, linex2, liney2, polx1, poly1, polx2, poly2)) {
			return false;
		}

	}
	flag1 = IsPointInPolygon(linex1, liney1, polygon);
	flag2 = IsPointInPolygon(linex2, liney2, polygon);

	return flag = flag1 * flag2;
}


int main()
{

	cout << "本程序可判断五种图形间的包含关系，需要判断哪种包含关系？请输入数字：" << endl;
	cout << "\n1：点在线段上\n2：点在多边形内部（不包括点在边上）\n3：线段1完全包含线段2\n4：线段在多边形内部（不包括邻接）\n5：多边形1完全包含多边形2（不包括邻接）\n" << endl;
	int choose;
	cin >> choose;
	while (choose > 0 && choose < 6) {
		if (choose == 1) {

			//输入点坐标
			cout << "输入要判断的点坐标" << endl;
			double pointx, pointy;
			cin >> pointx >> pointy;
			point P1(pointx, pointy);
			cout << "要判断的点坐标为" << endl;
			P1.Printpoint();

			cout << "请依次输入线段端点坐标X1,Y1,X2,Y2" << endl;
			double linex1, liney1, linex2, liney2;
			cin >> linex1 >> liney1 >> linex2 >> liney2;
			point linep1(linex1, liney1); point linep2(linex2, liney2);
			cout << "线段两端点坐标为" << endl;
			linep1.Printpoint();
			linep2.Printpoint();

			if (IsPointOnLine(pointx, pointy, linex1, liney1, linex2, liney2) == 1)
				cout << "点（" << P1.x << "," << P1.y << ")在线段上" << endl;
			else
				cout << "点（" << P1.x << "," << P1.y << ")不在线段上" << endl;
		}
		else if (choose == 2) {


			//输入点坐标
			cout << "输入要判断的点坐标" << endl;
			double pointx, pointy;
			cin >> pointx >> pointy;
			point P1(pointx, pointy);
			cout << "要判断的点坐标为" << endl;
			P1.Printpoint();


			//定义一个多边形
			cout << "请依次输入围成多边形的点坐标，从起点到终点" << endl;
			double inputx, inputy;
			vector<point> polygon;
			while (cin >> inputx >> inputy) {
				polygon.push_back(point(inputx, inputy));
				cout << "输入的点为（" << inputx << "," << inputy << ")" << endl;
				if (polygon.size() > 2) {
					if ((inputx == polygon[0].x) && (inputy == polygon[0].y)) {
						break;
					}
				}
			}
			cout << "\n围成多边形的点共有" << polygon.size() - 1 << "个" << endl;
			cout << "\n从起点到终点依次为" << endl;
			for (unsigned int i = 0; i < polygon.size(); i++) {
				polygon[i].Printpoint();
			}

			//判断
			if (IsPointInPolygon(P1.x, P1.y, polygon))
			{
				cout << "\n点（" << P1.x << "," << P1.y << ")在多边形内部" << endl;
			}
			else
			{
				cout << "\n点（" << P1.x << "," << P1.y << ")不在多边形内部" << endl;
			}
		}
		else if (choose == 3) {
			cout << "请依次输入线段1的两端点坐标X1,Y1,X2,Y2" << endl;
			double linex1, liney1, linex2, liney2;
			cin >> linex1 >> liney1 >> linex2 >> liney2;
			point linep1(linex1, liney1); point linep2(linex2, liney2);
			cout << "线段1两端点坐标为" << endl;
			linep1.Printpoint();
			linep2.Printpoint();

			cout << "请依次输入线段2的两端点坐标X3,Y3,X4,Y4" << endl;
			double linex3, liney3, linex4, liney4;
			cin >> linex3 >> liney3 >> linex4 >> liney4;
			point linep3(linex3, liney3); point linep4(linex4, liney4);
			cout << "线段2两端点坐标为" << endl;
			linep3.Printpoint();
			linep4.Printpoint();

			//判断
			if (IsLine1InLine2(linep1.x, linep1.y, linep2.x, linep2.y, linep3.x, linep3.y, linep4.x, linep4.y))
			{
				cout << "\n线段1包含线段2" << endl;
			}
			else
			{
				cout << "\n线段1不包含线段2" << endl;
			}



		}
		else if (choose == 4) {
			cout << "请依次输入线段的两端点坐标X1,Y1,X2,Y2" << endl;

			//输入线段端点
			double linex1, liney1, linex2, liney2;
			cin >> linex1 >> liney1 >> linex2 >> liney2;
			point linep1(linex1, liney1); point linep2(linex2, liney2);
			cout << "线段两端点坐标为" << endl;
			linep1.Printpoint();
			linep2.Printpoint();

			//定义一个多边形
			cout << "请依次输入围成多边形的点坐标，从起点到终点" << endl;
			double inputx, inputy;
			vector<point> polygon1;
			while (cin >> inputx >> inputy) {
				polygon1.push_back(point(inputx, inputy));
				cout << "输入的点为（" << inputx << "," << inputy << ")" << endl;
				if (polygon1.size() > 2) {
					if ((inputx == polygon1[0].x) && (inputy == polygon1[0].y)) {
						break;
					}
				}
			}
			cout << "\n围成多边形的点共有" << polygon1.size() - 1 << "个" << endl;
			cout << "\n从起点到终点依次为" << endl;
			for (unsigned int i = 0; i < polygon1.size(); i++) {
				polygon1[i].Printpoint();
			}

			//判断
			bool flag = IsLineInPolygon(linep1.x, linep1.y, linep2.x, linep2.y, polygon1);


			if (flag == 0)
				cout << "线段不在多边形内部" << endl;
			else
				cout << "线段在多边形内部" << endl;



		}
		else if (choose == 5) {

			//定义第一个多边形
			cout << "请依次输入围成第一个多边形的点坐标，从起点到终点" << endl;
			double inputx, inputy;
			vector<point> polygon1;
			while (cin >> inputx >> inputy) {
				polygon1.push_back(point(inputx, inputy));
				cout << "输入的点为（" << inputx << "," << inputy << ")" << endl;
				if (polygon1.size() > 2) {
					if ((inputx == polygon1[0].x) && (inputy == polygon1[0].y)) {
						break;
					}
				}
			}
			cout << "\n围成多边形1的点共有" << polygon1.size() - 1 << "个" << endl;
			cout << "\n从起点到终点依次为" << endl;
			for (unsigned int i = 0; i < polygon1.size(); i++) {
				polygon1[i].Printpoint();
			}
			//定义第二个多边形
			cout << "请依次输入围成第二个多边形的点坐标，从起点到终点" << endl;
			vector<point> polygon2;
			while (cin >> inputx >> inputy) {
				polygon2.push_back(point(inputx, inputy));
				cout << "输入的点为（" << inputx << "," << inputy << ")" << endl;
				if (polygon2.size() > 2) {
					if ((inputx == polygon2[0].x) && (inputy == polygon2[0].y)) {
						break;
					}
				}
			}
			cout << "\n围成多边形2的点共有" << polygon2.size() - 1 << "个" << endl;
			cout << "\n从起点到终点依次为" << endl;
			for (unsigned int i = 0; i < polygon2.size(); i++) {
				polygon2[i].Printpoint();
			}

			//判断
			bool flag = 1;
			for (unsigned int i = 0; i < polygon2.size() - 1; i++)
			{
				double polx1 = polygon2[i].x;
				double poly1 = polygon2[i].y;
				double polx2 = polygon2[i + 1].x;
				double poly2 = polygon2[i + 1].y;
				if (IsLineInPolygon(polx1, poly1, polx2, poly2, polygon1) == 0) {
					flag = 0;
				}
			}
			if (flag == 0)
				cout << "多边形1不包含多边形2" << endl;
			else
				cout << "多边形1完全包含多边形2" << endl;
		}
		cout << "\n\n请继续，需要判断哪种包含关系？请输入数字：" << endl;
		cout << "\n1：点在线段上\n2：点在多边形内部（不包括点在边上）\n3：线段1完全包含线段2\n4：线段在多边形内部（不包括邻接）\n5：多边形1完全包含多边形2（不包括邻接）\n" << endl;
		cin >> choose;
	}

	cout << "程序结束" << endl;
	return 0;
}