#include <stdio.h>
#include <math.h>

#define RAD(a) ((a * M_PI) / 180.0)

/*
 * Código para calcular a distância geográfica entre dois pontos dadas
 * as coordenadas de latitude e longitude de cada um deles. O cálculo
 * ignora variações de elevação, e assume que a superfície da Terra é
 * esférica.
 */

typedef struct {

	double x;
	double y;
	double z;

} Vector;

typedef struct {

	double latitude;
	double longitude;

} Coordinate;

Coordinate coordinate(double latitude, double longitude){
	
	Coordinate c;

	c.latitude = latitude;
	c.longitude = longitude;

	return c;
}

Vector vector(Coordinate c){

	Vector v;

	double lat = RAD(c.latitude);
	double lon = RAD(c.longitude);

	v.x = cos(lat) * sin(lon);
	v.y = sin(lat);
	v.z = cos(lat) * cos(lon);

	return v;
}

double dot(Vector v1, Vector v2){

	return 	v1.x * v2.x +
		v1.y * v2.y + 
		v1.z * v2.z
	;
}

int distance(Coordinate c1, Coordinate c2, double radius){

	Vector p1 = vector(c1);
	Vector p2 = vector(c2);

	double angle = acos(dot(p1, p2));
	double distance = angle * radius;

	return lrint(distance);
}

/*int main(){

	Coordinate cgh = coordinate(-23.626692,-46.655375);
	Coordinate gig = coordinate(-22.808903,-43.243647);
	Coordinate gru = coordinate(-23.432075,-46.469511); 
	Coordinate nrt = coordinate(35.764722,140.386389);
	Coordinate mad = coordinate(40.493556,-3.566764);
	double earth_radius =  6371.009;

	printf("Gongonhas - Guarulhos distance = %d km.\n", distance(cgh, gru, earth_radius));
	printf("Guarulhos - Narita distance = %d km.\n", distance(gru, nrt, earth_radius));
	printf("Congonhas - Galeao distance = %d km.\n", distance(cgh, gig, earth_radius));
	printf("Guarulhos - Madri distance = %d km.\n", distance(cgh, mad, earth_radius));

	return 0;
}*/

