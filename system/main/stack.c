/* Stack a dataset over one of the dimensions.

This operation is adjoint to sfspray.
*/
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

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <rsf.h>

int main(int argc, char* argv[])
{
    int j, n2, i2, ni, *fold = NULL, axis;
    off_t i, n, i3, n3;
    sf_file in, out;
    char key1[7];
    bool norm, rms, min, max, prod;
    float *trace, *stack;
    double *dstack;
    sf_datatype type;

    sf_init (argc, argv);

    in  = sf_input ( "in");
    out = sf_output("out");

    if (!sf_getint("axis",&axis)) axis=2;
    /* which axis to stack */

    n = 1;
    for (j=0; j < axis-1; j++) {
	sprintf(key1,"n%d",j+1);
	if (!sf_histint(in,key1,&ni)) break;
	n *= ni;
    }

    type = sf_gettype (in);
    if (SF_FLOAT != type) {
	if (SF_COMPLEX == sf_gettype (in)) {
	    n *= 2; 
	    /* possibly incorrect norm for complex data */
	} else {
	    sf_error("Incorrect data type in input");
	}
    }

    sprintf(key1,"n%d",axis);
    if (!sf_histint(in,key1,&n2)) sf_error("No %s= in input",key1);
    n3 = sf_unshiftdim(in,out,axis);
 
    if (!sf_getbool("rms",&rms)) rms = false;
    /* If y, compute the root-mean-square instead of stack. */
    if (rms || !sf_getbool("norm",&norm)) norm = true;
    /* If y, normalize by fold. */
    if (!sf_getbool("min",&min)) min = false;
    /* If y, find minimum instead of stack.  Ignores rms and norm. */
    if (!sf_getbool("max",&max)) max = false;
    /* If y, find maximum instead of stack.  Ignores rms and norm. */
    if (!sf_getbool("prod",&prod)) prod = false;
    /* If y, find product instead of stack.  Ignores rms and norm. */
    
    if (min || max || prod) { rms = false; norm = false; }
    if ((min && max) || (min && prod) || (max && prod)) 
	sf_error("Cannot have min=y max=y prod=y");

    if (norm) fold = sf_intalloc (n);
    trace = sf_floatalloc (n);
    stack = sf_floatalloc (n);
    dstack = (min || max)? NULL: (double*) sf_alloc(n,sizeof(double));

    for (i3=0; i3 < n3; i3++) {
        if (min || max) {
            if (min) for (i=0; i<n; i++) stack[i] =  FLT_MAX;
            if (max) for (i=0; i<n; i++) stack[i] = -FLT_MAX;
        } else {
	    for (i=0; i < n; i++) {
		dstack[i] = prod? 1.0:0.0;
	    }
	}
        if (norm) memset (fold,0,n*sizeof(int));
        
        for (i2=0; i2 < n2; i2++) {
            sf_floatread (trace, n, in);
            for (i=0; i < n; i++) {
                if (min || max) {
                    if (min) stack[i] = SF_MIN(stack[i],trace[i]);
                    if (max) stack[i] = SF_MAX(stack[i],trace[i]);
                } else if (prod) {
		    dstack[i] *= trace[i];
		} else {
		    dstack[i] += rms? (double) trace[i]*trace[i]: trace[i];
		}
                if (norm && (0.0 != trace[i])) fold[i]++; 
            }
        }
	if (!min && !max) {
	    for (i=0; i < n; i++) {
		if (norm) {
		    if (fold[i] > 0) {
			dstack[i] /= fold[i];
			if (rms) dstack[i] = sqrt(dstack[i]);
		    }
		}
		stack[i] = dstack[i];
	    }
	}
	    	
        sf_floatwrite(stack, n, out); 
    }


    exit (0);
}

/* 	$Id$	 */
