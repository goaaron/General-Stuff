/*
* The four routines for anova splitting
*/
#include "rpart.h"
#include "rpartproto.h"

static double *mean, *sums, *treatsums;
static double * ceffect; // \tau(1) - \tau(0) in one leaf, which will be our return
static double *wts;
static double *treats; // add s to differentiate with treat
static int *countn;
static int *tsplit;

int
anovainit(int n, double *y[], double *treat, int maxcat, char **error,
	  double *parm, int *size, int who, double *wt)
{
    if (who == 1 && maxcat > 0) {
	    graycode_init0(maxcat);
	    countn = (int *) ALLOC(2 * maxcat, sizeof(int));
	    tsplit = countn + maxcat;
	    mean = (double *) ALLOC(6 * maxcat, sizeof(double));
	    wts = mean + maxcat;
      treats = wts + maxcat;
	    sums = wts + maxcat;
      treatsums = sums + maxcat;
      ceffect = sums + maxcat; // what we gonna return 
      
    }
    *size = 1;
    return 0;
}

/*
* The anova evaluation function.  Return the mean and the ss.
*/
void
anovass(int n, double *y[], double *treat, double *value, double *risk, double *wt, double max_y)
{
    int i;
    double temp = 0., twt = 0.; /* sum of the weights */
    double ttreat = 0., tcontrol = 0.; /* sum of the treated cases, controlled cases */
    double mean, tmean, cmean, ceffect, ss;
    double temp1 = 0., temp0 = 0.; /* temp variables for treated and controlled cases */
    
    for (i = 0; i < n; i++) {
      temp += *y[i] * wt[i];
      twt += wt[i];
      if (treat[i] == 1) {
        temp1 += *y[i];
        ttreat += 1;
      } else {
        temp0 += *y[i];
        tcontrol += 1;
      }
    }
    mean = temp / twt;
    tmean = temp1 / ttreat;
    cmean = temp0 / tcontrol;
    ceffect = tmean - cmean;

    ss = 0;
    /*
    for (i = 0; i < n; i++) {
      temp = *y[i] - mean;
	    ss += temp * temp * wt[i];
    }
    */

    *value = ceffect;
    /* 4 * n * max_y^2 is our upperbound of ss causal effects */
    *risk = 4 * n * max_y * max_y - ceffect *ceffect * n;   
}

/*
 * The anova splitting function.  Find that split point in x such that
 *  the sum of squares of y within the two groups is decreased as much
 *  as possible.  It is not necessary to actually calculate the SS, the
 *  improvement involves only means in the two groups.
 */

void
anova(int n, double *y[], double *treat, double *x, int nclass,
      int edge, double *improve, double *split, int *csplit,
      double myrisk, double *wt, double min_node_size)
{
    int i, j;
    double temp;
    double left_sum, right_sum;
    double left_treat_sum, right_treat_sum;
    double left_wt, right_wt;
    double left_treat, right_treat; 
    /* number of treated cases in leftson and rightson */
    int left_n, right_n;
    /* number of samples in leftson and rightson */
    double grandmean, best; 
    double node_effect, left_effect, right_effect;
    double tempeffect;
    // double ceffect; // causal effect 
    int direction = LEFT;
    int where = 0;

   /*
    * The improvement of a node is SS - (SS_L + SS_R), where
    *   SS = sum of squares in a node = \sum w_i (x_i - \bar x)^2, where
    * of course \bar x is a weighted mean \sum w_i x_i / \sum w_i
    * Using the identity
    *    \sum w_i(x_ - \bar x)^2 = \sum w_i (x_i-c)^2 - (\sum w_i)(c-\bar x)^2
    * the improvement = w_l*(left mean - grand mean)^2
    *                  +w_r*(right mean- grand mean)^2
    * where w_l is the sum of weights in the left node, w_r similarly.
    */
    
    /* The improvement of a node is left_sse + right_sse - node_sse, where 
     * 
     */
    right_wt = 0;
    right_treat = 0;
    right_n = n;
    right_sum = 0;
    right_treat_sum = 0;
    for (i = 0; i < n; i++) {
	    right_sum += *y[i] * wt[i];
	    right_wt += wt[i];
      right_treat += treat[i];
      right_treat_sum += *y[i] * treat[i];
    }
    grandmean = right_sum / right_wt;
    tempeffect = (right_treat_sum - right_sum * right_treat / n) /
                 ((1 - right_treat / n) * right_treat);
  
    node_effect = tempeffect * tempeffect * n;

    if (nclass == 0) {          /* continuous predictor */
      left_sum = 0;           /* No data in left branch, to start */
      left_treat_sum = 0;
	    left_wt = 0;
      left_treat = 0;
	    left_n = 0;
	    right_sum = 0;          /* after subracting grand mean, it's zero */
      right_treat_sum = 0;
	    best = 0;
	    for (i = 0; right_n > edge; i++) {
	      left_wt += wt[i];
	      right_wt -= wt[i];
        left_treat += treat[i];
        right_treat -= treat[i];
	      left_n++;
	      right_n--;
        
        temp = *y[i] * treat[i];
        left_treat_sum += temp;
        right_treat_sum -= temp;
        
        temp = *y[i] * wt[i];
        left_sum += temp;
        right_sum -= temp;
        
	      //temp = (*y[i] - grandmean) * wt[i];
	      //left_sum += temp;
	      //right_sum -= temp;
        
        // where min_node_size show its usage 
	      if (x[i + 1] != x[i] && left_n >= edge &&
            left_treat >= min_node_size &&
            left_n - left_treat >= min_node_size &&
            right_treat >= min_node_size &&
            right_n - right_treat >= min_node_size) {
              tempeffect = (left_treat_sum - left_sum * left_treat / left_n) /
                 ((1 - left_treat / left_n) * left_treat);
              left_effect = tempeffect * tempeffect * left_n;
              
              tempeffect = (right_treat_sum - right_sum * right_treat / right_n) /
                 ((1 - right_treat / right_n) * right_treat);
              right_effect = tempeffect * tempeffect * right_n;
          
              temp = left_effect + right_effect - node_effect;
              
              
              // temp = left_sum * left_sum / left_wt + 
              // right_sum * right_sum / right_wt;
              
              if (temp > best) {
                best = temp;
                where = i;
                // set a criterion for going direction
		            if (left_sum < right_sum)
                  direction = LEFT;
		            else
			            direction = RIGHT;
		          }
            }
	    }
      
      *improve = best / myrisk;
      
      if (best > 0) {         /* found something */
        csplit[0] = direction;
	      *split = (x[where] + x[where + 1]) / 2;
	    }
    }
    
    /*
     * Categorical predictor
     */
     
    else {
      for (i = 0; i < nclass; i++) {
	    sums[i] = 0;
	    countn[i] = 0;
	    wts[i] = 0;
      treats[i] = 0;
      }

       /* rank the classes by their mean y value */
       for (i = 0; i < n; i++) {
	        j = (int) x[i] - 1;
	        countn[j]++;
	        wts[j] += wt[i];
          treats[j] += treat[j];
	        sums[j] += (*y[i] - grandmean) * wt[i];
          
	      }
	    for (i = 0; i < nclass; i++) {
	      if (countn[i] > 0) {
		      tsplit[i] = RIGHT;
		      mean[i] = sums[i] / wts[i];
	      } else
		      tsplit[i] = 0;
	    }
	    graycode_init2(nclass, countn, mean);

	/*
	 * Now find the split that we want
	 */
	    left_wt = 0;
      left_treat = 0;
	    left_sum = 0;
      left_treat_sum  = 0;
	    right_sum = 0;
      right_treat_sum = 0;
	    left_n = 0;
	    best = 0;
	    where = 0;
      
	    while ((j = graycode()) < nclass) {
	      tsplit[j] = LEFT;
	      left_n += countn[j];
	      right_n -= countn[j];
	      left_wt += wts[j];
        left_treat += treats[j];
	      right_wt -= wts[j];
        right_treat -= treats[j];
	      left_sum += sums[j];
	      right_sum -= sums[j];
        left_treat_sum += treatsums[j];
        right_treat_sum -= treatsums[j];
	      if (left_n >= edge && right_n >= edge &&
            left_treat >= min_node_size &&
            left_n - left_treat >= min_node_size &&
            right_treat >= min_node_size &&
            right_n - right_treat >= min_node_size) {
              
              tempeffect = (left_treat_sum - left_sum * left_treat / left_n) /
                 ((1 - left_treat / left_n) * left_treat);
              left_effect = tempeffect * tempeffect * left_n;
              
              tempeffect = (right_treat_sum - right_sum * right_treat / right_n) /
                 ((1 - right_treat / right_n) * right_treat);
              right_effect = tempeffect * tempeffect * right_n;
          
              temp = left_effect + right_effect - node_effect;
            
              // temp = left_sum * left_sum / left_wt +
		          // right_sum * right_sum / right_wt;
		          if (temp > best) {
		            best = temp;
                // which patch goes to the left direction ----- may need to reorder
		            if ((left_sum / left_wt) > (right_sum / right_wt))
                  for (i = 0; i < nclass; i++) csplit[i] = -tsplit[i];
		            else
                  for (i = 0; i < nclass; i++) csplit[i] = tsplit[i];
              }
        }
      }
      
      *improve = best / myrisk;       /* % improvement */
      }
}
