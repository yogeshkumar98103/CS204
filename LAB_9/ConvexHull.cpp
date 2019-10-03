//
// Created by Yogesh Kumar on 2019-10-03.
//

#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
using namespace std;

struct Point{
    int x;
    int y;
};

enum Orientation{
    Collinear,
    LeftRotation,
    RightRotation,
};

Orientation orientation(Point p1, Point p2, Point p3){
    long long res = (p2.y - p1.y)*(p3.x - p2.x) - (p2.x - p1.x)*(p3.y - p2.y);
    if(res == 0) return Collinear;
    if(res > 0) return RightRotation;
    return LeftRotation;
}

long long dist(Point p1, Point p2){
    return (long long)(p1.x - p2.x)*(p1.x - p2.x) + (long long)(p1.y - p2.y)*(p1.y - p2.y);
}

// Comparison Class
class SortFunc{
    Point pRef;
public:
    SortFunc(Point pRef){
        this->pRef = pRef;
    }

    bool operator()(Point p1, Point p2){
        Orientation orient = orientation(pRef, p1, p2);
        if(orient == Collinear){
            return dist(p1, pRef) < dist(p2, pRef);
        }
        return orient == LeftRotation;
    }
};

Point nextToTop(stack<Point>& s){
    Point top = s.top();
    s.pop();
    Point ret = s.top();
    s.push(top);
    return ret;
}

vector<Point> findConvexHull(vector<Point> points){
    // Find Min X Point
    int min = 0;

    int x,y;
    for(int i = 1; i < points.size(); i++){
        x = points[i].x;
        y = points[i].y;
        if(x < points[min].x || (x == points[min].x && y < points[min].y)){
            min = i;
        }
    }

    swap(points[min], points[0]);

    // Sort Points According to inclination w.r.t bottom most point
    sort(points.begin() + 1, points.end(), SortFunc(points[0]));

    // Delete all collinear points
    int m = 1;
    for(int i = 1; i < points.size(); i++){
        while(i < points.size() - 1 && orientation(points[0], points[i], points[i+1]) == Collinear)
            i++;
        points[m] = points[i];
        m++;
    }

    if(m < 3) return {};

    stack<Point> s;
    s.push(points[0]);
    s.push(points[1]);
    s.push(points[2]);

    // Find Other Vertices of Conves Hull
    for(int i = 3; i < m; i++){
        while(orientation(nextToTop(s), s.top(), points[i]) != LeftRotation){
            s.pop();
        }
        s.push(points[i]);
    }

    vector<Point> convexHull;
    while(!s.empty()){
        convexHull.push_back(s.top());
        s.pop();
    }
    rotate(convexHull.begin(), convexHull.begin() + convexHull.size() -1, convexHull.end());
    return convexHull;
}

int main(){
    // Input
    int n;
    cin >> n;
    vector<Point> points(n);
    for(int i = 0; i < n; i++){
        cin >> points[i].x >> points[i].y;
    }

    // Find ConvexHull
    vector<Point> convexHull = findConvexHull(points);

    // Output
    cout << convexHull.size() << endl;
    for(Point p: convexHull){
        cout << p.x << " " <<  p.y << endl;
    }
    return 0;
}
