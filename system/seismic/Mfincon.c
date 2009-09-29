/* Offset continuation by finite differences */
/*
  Copyright (C) 2004 University of Texas at Austin

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <rsf.h>
#include "ctridiagonal.h"

int main(int argc, char* argv[])
{
    int nw,nh,nx, iw,ix,ih, k;
    float dw, h0,dh,dx, w0,w,w2, h,h2;
    sf_complex diag, diag2, *in=NULL, *out=NULL, offd, offd2, c1, c2;
    bool all;
    ctris slv;
    sf_file input=NULL, output=NULL;

    sf_init (argc,argv);
    input = sf_input("in");
    output = sf_output("out");

    if (SF_COMPLEX != sf_gettype(input)) sf_error("Need complex input");

    if (!sf_histint(input,"n1",&nx)) sf_error("No n1= in input");
    if (!sf_histint(input,"n2",&nw)) sf_error("No n2= in input");
    if (!sf_histfloat(input,"d1",&dx)) sf_error("No d1= in input");
    if (!sf_histfloat(input,"d2",&dw)) sf_error("No d2= in input");
    if (!sf_histfloat(input,"o2",&w0)) sf_error("No o2= in input");

    if (!sf_getint("nh",&nh)) sf_error("Need nh=");
    /* Number of steps in offset */
    if (!sf_getfloat("dh",&dh)) sf_error("Need dh=");
    /* Offset step size */
    if (!sf_getfloat("h0",&h0)) sf_error("Need h0=");
    /* Initial offset */

    if (!sf_getbool("all",&all)) all=false;
    /* if y, output all offsets */

    if (all) {
	sf_putint(output,"n2",nh+1);
	sf_putfloat(output,"d2",dh);
	sf_putfloat(output,"o2",h0);
	sf_putint(output,"n3",nw);
	sf_putfloat(output,"d3",dw);
	sf_putfloat(output,"o3",w0);
    }

    dh /= dx;
    h0 /= dx;

    w0 *= 2.*SF_PI;
    dw *= 2.*SF_PI;

    in = sf_complexalloc(nx);
    out = sf_complexalloc(nx);

    slv = ctridiagonal_init (nx);

    for (iw=0; iw < nw; iw++) {
	sf_warning("frequency %d of %d",iw+1,nw);

	w = w0+iw*dw; 
	w2 = w*w;

	sf_complexread(out,nx,input);

	if (fabsf(w) < dw) {
	    for (ix=0; ix < nx; ix++) {
		out[ix]=sf_cmplx(0.,0.);
	    }
	    if (all) {
		for (ih=0; ih < nh; ih++) {
		    sf_complexwrite (out,nx,output);
		}
	    } 
	    sf_complexwrite (out,nx,output);
	    continue;
	}

#ifdef SF_HAS_COMPLEX_H
	c1 = 3.*sf_cmplx(9. + w2,4.*w)/(w2*sf_cmplx(3.,- w));
	c2 = 3.*sf_cmplx(w2 - 27.,8.*w)/(w2*sf_cmplx(3.,- w));
#else
	c1 = sf_cdiv(sf_cmplx(3.*(9. + w2),3.*4.*w),
		     sf_cmplx(w2*3.,-w2*w));
	c2 = sf_cdiv(sf_cmplx(3.*(w2 - 27.),3.*8.*w),
		     sf_cmplx(w2*3.,-w2*w));
#endif

	for (ih=0; ih < nh; ih++) {
	    if (all) sf_complexwrite (out,nx,output);

	    for (ix=0; ix < nx; ix++) {
		in[ix] = out[ix];
	    }

	    h = h0 + ih*dh; 
	    h2 = h+dh;
	    h *= h;
	    h2 *= h2; 

#ifdef SF_HAS_COMPLEX_H	
	    offd  = 1. - c1*h2 + c2*h;
	    offd2 = 1. - c1*h  + c2*h2;
	    diag  = 12. - 2.*offd;
	    diag2 = 12. - 2.*offd2;
#else
	    offd  = sf_cadd(sf_cmplx(1.,0.),
			    sf_cadd(sf_crmul(c1,-h2),
				    sf_crmul(c2,h)));
	    offd2 = sf_cadd(sf_cmplx(1.,0.),
			    sf_cadd(sf_crmul(c1,-h),
				    sf_crmul(c2,h2)));
	    diag  = sf_cadd(sf_cmplx(12.,0.),sf_crmul(offd,-2.));
	    diag2 = sf_cadd(sf_cmplx(12.,0.),sf_crmul(offd2,-2.));
#endif

	    ctridiagonal_const_define (slv, diag2, offd2);

#ifdef SF_HAS_COMPLEX_H	
	    out[0] = diag*in[0] + offd*in[1];
#else
	    out[0] = sf_cadd(sf_cmul(diag,in[0]),
			     sf_cmul(offd,in[1]));
#endif
	    for (k = 1; k < nx - 1; k++) {
#ifdef SF_HAS_COMPLEX_H	
		out[k] = diag*in[k] + offd*(in[k+1]+in[k-1]);
#else
		out[k] = sf_cadd(sf_cmul(diag,in[k]),
				 sf_cmul(offd,sf_cadd(in[k+1],in[k-1])));
#endif
	    }
#ifdef SF_HAS_COMPLEX_H	
	    out[nx-1] = diag*in[nx-1] + offd*in[nx-2];
#else
	    out[nx-1] = sf_cadd(sf_cmul(diag,in[nx-1]),
				sf_cmul(offd,in[nx-2]));
#endif
	    ctridiagonal_solve (slv, out);
	}
	sf_complexwrite (out,nx,output);
    }

    exit(0);
}
