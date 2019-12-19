
void calculateSum(int *nrows, long *bigMatrix,
                  int *startrow, long *totalSum, pthread_mutex_t *mutex);
void calaulerProduit(int *nrows, pthread_mutex_t *mutex,
                     int *startrow, long *tempMatrix,
                     long *poidMatrix, long *tempoidMatrix);
void weightedSum(int *nrows, pthread_mutex_t *mutex,
                 int *startrow, long *tempMatrix,
                 long *poidMatrix, long *WeithedSum);