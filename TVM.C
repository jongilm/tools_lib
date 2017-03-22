#include <stdlib.h>
#include <math.h>

/*
   zero = PV + (1+I*P) * PMT * (1-pow(1+I,-N)) / I + FV * pow(1+I,-N);
   printf("Zero ==> %-16.4f\n",zero);
*/

double tvm_pmt(double PV,double FV,double I,double N,double PMT,double P)
{
   I   = I/1200.0;
   PMT = -((I * (PV + FV * pow(1+I,-N) ) / (1+I*P))/(1-pow(1+I,-N)));
   return(PMT);
}

double tvm_pv (double PV,double FV,double I,double N,double PMT,double P)
{
   I   = I/1200.0;
   PV = -((1+I*P) * PMT * (1-pow(1+I,-N)) / I + FV * pow(1+I,-N));
   return(PV);
}

double tvm_fv (double PV,double FV,double I,double N,double PMT,double P)
{
   I   = I/1200.0;
   FV  =-(PV + (1+I*P) * PMT * (1-pow(1+I,-N)) / I) / pow(1+I,-N);
   return(FV);
}

double tvm_n  (double PV,double FV,double I,double N,double PMT,double P)
{
   I   = I/1200.0;
   N = -((log(((1+I*P)*PMT+I*PV)/((1+I*P)*PMT-I*FV)))/(log(1+I)));
   return(N);
}

double tvm_i  (double PV,double FV,double I,double N,double PMT,double P)
{
   double di;

   I = fabs(PMT/PV) - fabs(PV/PMT)/pow(N,2);

   do
   {
      di = ( I*fabs(PV/PMT) - (1-pow(I+1,-N)) )/
          ( ( pow(I+1,-N) * (1+N/(1+1/I)) - 1)/I );

      I += di;
   } while (0.000001 <= fabs(di));

   return(I*1200.0);
}
