#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct cluster{
    float *sum;
    int size;
};

float ** init_cent(float **prev_cent, float **obs, int k, int d);
void init_clusters(struct cluster * clusters, int k, int d);
int find_minimum(const float *p, int k);
int is_equal(float **p1, float **p2, int k, int d);
void copy_array(float **p1, float **p2, int k, int d);
struct cluster;
void reset_clusters(struct cluster* s_array,int k,int d);
float euc_dis(const float *p1,const float *p2,const int d);
void add_obs_to_cluster(float *curr_obs,struct cluster s,int d);
void update_cent(float **curr_cent, struct cluster* clusters, int k, int d);
void printCent(float **curr_cent, int k, int d);

int main(int argc, char* argv[]) {

    int k, n, d, MAX_ITER;
    int i;


    char arr1[20000];
    const char del[2] = ",";
    int j;
    int counter;
    struct cluster * clusters;
    float **prev_cent;
    float **curr_cent;
    float **observations;
    k = atoi(argv[1]);
    n = atoi(argv[2]);
    d = atoi(argv[3]);
    MAX_ITER = atoi(argv[4]);
    if (k <= 0 || n <= 0 || d <= 0 || MAX_ITER <= 0 || n <= k ) {
        printf("Check the given arguments");
        return 0;
    }
    if (argc != 5){
        printf("Not enough arguments given");
        return 0;
    }
    /*allocate memory for observations array*/

    observations = (float **) malloc(n * sizeof(float *));
    assert(observations != NULL);


    for (i = 0; i < n; i++) {
        observations[i] = (float *) malloc(d * sizeof(float));
        assert(observations[i] != NULL);
    }


    j = 0;

    while (j < n && (scanf("%s", arr1) == 1)) {
        int m;
        int tokenCounter;
        char *token;
        m = 0;
        token = strtok(arr1, del);
        observations[j][m] = (float)atof(token);
        tokenCounter = 1;
        while( tokenCounter < d ) {
            m++;
            token = strtok(NULL, del);
            observations[j][m] = (float)atof(token);
            tokenCounter++;
        }
        j++;

    }
    /* malloc arrays */
    clusters=(struct cluster*)malloc(k* sizeof(struct cluster));
    assert(clusters != NULL);
    /* malloc inside arrays */

    prev_cent = (float **) malloc(k  * sizeof(float *));
    assert(prev_cent != NULL);
    curr_cent = (float **) malloc(k  * sizeof(float *));
    assert(curr_cent != NULL);
    /* malloc inside arrays */
    for (i = 0; i < k; i++) {
        prev_cent[i] = (float *) malloc(d * sizeof(float));
        assert(prev_cent[i] != NULL);
        for (j = 0; j < d; j++){
            prev_cent[i][j] = 0;
        }
        curr_cent[i] = (float *) malloc(d * sizeof(float));
        assert(curr_cent != NULL);
    }

    curr_cent = init_cent(curr_cent, observations, k, d);

    init_clusters(clusters, k, d);
    counter = 0;

    /* repeat until: */
    while (counter < MAX_ITER && !is_equal(prev_cent, curr_cent, k, d)) {
        int a, b;
        copy_array(prev_cent, curr_cent, k, d);
        reset_clusters(clusters,k,d);
        for (a = 0; a < n; a++) {
            int index;
            float *curr_obs = observations[a];
            float *dist = (float *)calloc(k, sizeof(float));
            for (b = 0; b < k; b++) {
                dist[b] = euc_dis(curr_obs, prev_cent[b], d);
            }
            index = find_minimum(dist, k);
            add_obs_to_cluster(curr_obs,clusters[index],d);
            clusters[index].size++;

        }
        update_cent(curr_cent,clusters,k,d);
        counter++;

    }
    printCent(curr_cent, k, d);

    /* Free the memory used */
    for (i = 0; i < n; i++) {
        free(observations[i]);
        observations[i] = NULL;
    }
    free(observations);
    observations = NULL;

    for (i = 0; i < k; i++) {
        free(prev_cent[i]);
        free(curr_cent[i]);
        prev_cent[i] = NULL;
        curr_cent[i] = NULL;

    }
    free(prev_cent);
    free(curr_cent);
    prev_cent = NULL;
    curr_cent = NULL;

    for (j=0;j<k; j++){
        free(clusters[j].sum);
        clusters[j].sum = NULL;
    }
    free(clusters);
    clusters = NULL;

    return 1;
}

float ** init_cent(float **prev_cent, float **obs, int k, int d) {
    int i,j;
    for (i = 0; i < k; i++){
        for(j=0; j < d; j++){
            prev_cent[i][j] = obs[i][j];
        }
    }
    return prev_cent;
}

/* return the index of the min value in array*/
int find_minimum(const float *p, int k) {
    int c, index;
    float min;
    index = 0;
    min = p[0];
    for(c=0;c<k;c++){
        if (p[c] < min){
            index = c;
            min = p[c];
        }
    }
    return index;


}

/* checking if 2- array are equal*/
int is_equal(float **p1, float **p2, int k, int d){
    int i;
    int j;
    for (j=0;j<k; j++){
        for (i=0;i<d;i++){
            if (p1[j][i]!= p2[j][i])
                return 0;
        }
    }
    return 1;
}

/*copy the second array to the first one */
void copy_array(float **p1, float **p2, int k, int d){
    int i,j;
    for (j=0;j<k; j++){
        for (i = 0; i < d; i++) {
            p1[j][i] = p2[j][i];
        }
    }
}

/* reset clusters sum and size */
void reset_clusters(struct cluster* s_array,int k,int d ){
    int i,j;
    for (j=0;j<k; j++){
        s_array[j].size=0;
        for (i=0;i<d; i++) {
            s_array[j].sum[i] = 0;
        }
    }
}

/* calculate the distance between 2 vectors */
float euc_dis(const float *p1, const float *p2,const int d){
    float distance;
    int i;
    distance = 0;
    for (i =0; i< d; i++){
        distance += (p1[i]- p2[i])*(p1[i]- p2[i]);
    }
    return distance;
}

/* add the curr observation to the min cluster- update size and sum */
void add_obs_to_cluster(float *curr_obs,struct cluster s,int d){
    int i;
    for (i=0;i<d; i++){
        s.sum[i] += curr_obs[i];
    }
}
/* update new centroids */
void update_cent(float **curr_cent, struct cluster* clusters, int k, int d){
    int i,j;
    for(i=0; i<k; i++){
        for (j=0;j<d;j++){
            curr_cent[i][j] = clusters[i].sum[j]/ (float)clusters[i].size;
        }
    }
}

void printCent(float **curr_cent, int k, int d) {
    int centNumber;
    int centCord;
    for (centNumber = 0; centNumber < k; centNumber++){
        for (centCord = 0; centCord < d; centCord++){
            printf("%.2f", curr_cent[centNumber][centCord]);
            if (centCord != d-1){
                printf(", ");
            }
        }
        printf("\n");
    }

}

void init_clusters(struct cluster * clusters, int k, int d){
    int i,j;
    for (j=0;j<k; j++){
        struct cluster c;
        c.size=0;
        c.sum = (float *) malloc(d * sizeof(float));
        assert(c.sum != NULL);
        for (i=0;i<d; i++) {
            c.sum[i] = 0;
        }
        clusters[j]= c;
    }
}

