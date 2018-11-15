#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Coordinates
// 0,0 is the cuisse joint
// y is forward(+) backward(-) axe
// z is up down axe. Under the cuisse joint is negative

double Sin(double a)
{
	return sin(a*M_PI/180);
}

double Cos(double a)
{
	return cos(a*M_PI/180);
}

double Acos(double a)
{
	return acos(a)*180/M_PI;
}

// return coord corresponding to angles set for servos
int angleToCoord(int e, int r, int p)
{
  int lgPied = 65;
  int angCuissePied = 20; // angle du servo 0 par rapport à la verticale
  int lgCuisse = 68;
  int angVertCuisse = 90; // angle du servo 0 par rapport à la verticale
  double yr, zr, yp, zp, y, z;

/*
//  70 => y = lgPied
//  160 => y = 0
//  180 => y < 0
  yp = Cos(-90+lgPied+p)*lgPied;
  zp = Sin(90+lgPied+p)*lgPied;
  printf("yp=%f zp=%f\n", yp, zp);
*/
  yr = Cos(r)*lgCuisse;
  zr = Sin(-r)*lgCuisse;
  printf("yr=%f zr=%f\n", yr, zr);
  y = yr+Sin(180-(r-90)-(angCuissePied+p))*lgPied;
  z = zr-Cos(180-(r-90)-(angCuissePied+p))*lgPied;
  printf("y=%f z=%f\n", y, z);
	
}

#if 0
// return servo instructions from coord
int coordToAngle(double x, double y, double z)
{
  int lgPied = 65;
  int angCuissePied = 20; // angle du servo 0 par rapport à la verticale
  int lgCuisse = 68;
  int angVertCuisse = 90; // angle du servo 0 par rapport à la verticale
  double yr, zr, yp, zp, y, z;

  y = Cos(r)*lgCuisse+Sin(180-(r-90)-(angCuissePied+p))*lgPied;
  z = Sin(-r)*lgCuisse-Cos(180-(r-90)-(angCuissePied+p))*lgPied;

  y = Cos(r)*lgCuisse+Sin(270-r-angCuissePied-p)*lgPied;
  y-Cos(r)*lgCuisse = Sin(270-r-angCuissePied-p)*lgPied;
  270-r-angCuissePied-p = asin((y-Cos(r)*lgCuisse)/lgPied)
  p=270-r-angCuissePied-asin((y-Cos(r)*lgCuisse)/lgPied)

z = Sin(-r)*lgCuisse-Cos(270-r-angCuissePied-(270-r-angCuissePied-asin((y-Cos(r)*lgCuisse)/lgPied)))*lgPied
z = Sin(-r)*lgCuisse-Cos(270-r-angCuissePied-270+r-angCuissePied+asin((y-Cos(r)*lgCuisse)/lgPied))*lgPied
z = Sin(-r)*lgCuisse-Cos(asin((y-Cos(r)*lgCuisse)/lgPied))*lgPied

}
#endif

// return coord corresponding to angles set for servos
int getAnglePied(int z, int r)
{
  int lgPied = 65;
  int angCuissePied = 20; // angle du servo 0 par rapport à la verticale
  int lgCuisse = 68;
  int angVertCuisse = 90; // angle du servo 0 par rapport à la verticale
  double zr;
  int p;

  zr = Sin(-r)*lgCuisse;
  p = 270-r-angCuissePied-Acos(-(z-zr)/lgPied);
  printf("getAnglePied(%d, %d) => zr=%f p=%d\n", z, r, zr, p);
/*
  printf("180-(r-90)-(angCuissePied+p)=%d\n", 180-(r-90)-(angCuissePied+60));
  printf("cos(180-(r-90)-(angCuissePied+p))=%f\n",Cos(180-(r-90)-(angCuissePied+60)));
  printf("cos()*lgPied=%f\n",Cos(180-(r-90)-(angCuissePied+60))*lgPied);
  zr = Sin(-r)*lgCuisse;
  z = zr-Cos(180-(r-90)-(angCuissePied+p))*lgPied;
  z = zr-Cos(270-r-angCuissePied-p)*lgPied;
  Cos(270-r-angCuissePied-p) = (zr-z)/lgPied
  270-r-angCuissePied-p = acos((zr-z)/lgPied)
  p = 270-r-angCuissePied-acos((zr-z)/lgPied)
*/
  return p;
}


int main(int argc, char *argv[])
{
	int	r, p;

	if (argc < 2)
	{
		printf("use: tst.x angleRota anglePied\n");
		exit(1);
	}

//	printf("cos 0=%f 90=%f 180=%f 270=%f 360=%f\n", Cos(0), Cos(90), Cos(180), Cos(270), Cos(360));
//	printf("sin 0=%f 90=%f 180=%f 270=%f 360=%f\n", Sin(0), Sin(90), Sin(180), Sin(270), Sin(360));
	angleToCoord(0, atoi(argv[1]), atoi(argv[2]));

//	for (r=60; r<61; r++)
	for (r=60; r<120; r++)
	{
		p = getAnglePied(-35, r);
		angleToCoord(0, r, p);
	}
		
	return 0;
}
