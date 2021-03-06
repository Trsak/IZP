/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap) {
    assert(c != NULL);
    assert(cap >= 0);

    c->capacity = cap;
    c->size = 0;
    c->obj = NULL;

    if (cap > 0) {
        c->obj = malloc(cap * sizeof(struct obj_t));

        if (c->obj == NULL) {
            c->capacity = 0;
        }
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c) {
    free(c->obj);
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap) {
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj) {
    if (c->size >= c->capacity) {
        resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    }

    c->obj[c->size] = obj;

    ++c->size;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++) {
        append_cluster(c1, c2->obj[i]);
    }

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx) {
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&carr[idx]);

    for (int i = idx; i < narr - 1; i++) {
        carr[i] = carr[i + 1];
    }

    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2) {
    assert(o1 != NULL);
    assert(o2 != NULL);

    float x = o2->x - o1->x;
    float y = o2->y - o1->y;

    return sqrtf((x * x) + (y * y));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2) {
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float distance;
    float biggestDistance = 0.0;

    for (int i = 0; i < c1->size; ++i) {
        for (int j = 0; j < c2->size; ++j) {
            distance = obj_distance(&c1->obj[i], &c2->obj[j]);
            if (distance > biggestDistance) {
                biggestDistance = distance;
            }
        }
    }

    return biggestDistance;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2) {
    assert(narr > 0);

    float distance;
    float lowestDistance = INT_MAX;

    for (int i = 0; i < narr; ++i) {
        for (int j = 0; j < narr; ++j) {
            if (i != j) {
                distance = cluster_distance(&carr[i], &carr[j]);
                if (distance < lowestDistance) {
                    lowestDistance = distance;
                    *c1 = i;
                    *c2 = j;
                }
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b) {
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c) {
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++) {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr) {
    assert(arr != NULL);

    FILE *file;
    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Couldn't open file: %s!\n", filename);
        *arr = NULL;
        return 0;
    }

    int c = 0, rows = 0, position = 0;
    const char count[] = "count=";

    while (c != '\n') {
        c = getc(file);
        if (position > 5) {
            int num = c - 48;
            if (-1 < num && num < 10) {
                rows *= 10;
                rows += num;
            }
        } else {
            if (count[position] != c) {
                fprintf(stderr, "ERROR: Bad file format, count not found!\n");
                *arr = NULL;
                return 0;
            }
        }
        ++position;
    }

    struct cluster_t *clusters = malloc(rows * sizeof(struct cluster_t));
    *arr = clusters;
    if (clusters == NULL) {
        *arr = NULL;
        return rows;
    }

    int row = 1, id;
    float x, y;
    char endChar;

    while (rows >= row) {
        int n = fscanf(file, "%i%f%f%c", &id, &x, &y, &endChar);

        if (n == 4 || (n == 3 && rows == row)) {
            if (endChar == '\n' || endChar == '\r' || endChar == EOF) {
                struct cluster_t cluster;
                init_cluster(&cluster, 1);
                cluster.size = 1;

                if (x < 0.0 || x > 1000.0 || y < 0.0 || y > 1000.0) {
                    fprintf(stderr, "ERROR: Object coords must be bigger then 0 and less then 1000!\n");
                    *arr = NULL;

                    return 0;
                }

                clusters[row - 1] = cluster;
                clusters[row - 1].obj->id = id;
                clusters[row - 1].obj->x = x;
                clusters[row - 1].obj->y = y;
                ++row;
            } else {
                *arr = NULL;
                fprintf(stderr, "ERROR: Bad file format!\n");
                break;
            }
        } else {
            *arr = NULL;
            fprintf(stderr, "ERROR: There is not %d objects in the file!\n", rows);
            break;
        }
    }

    fclose(file);
    return rows;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr) {
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++) {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[]) {
    int N = 1;

    if (argc == 3 || argc == 2) {
        if (argc == 3) {
            char *ptr;
            N = strtol(argv[2], &ptr, 10);

            if (ptr != 0 && N <= 0) {
                fprintf(stderr, "ERROR: N must be Integer bigger then zero!\n");
                return EXIT_FAILURE;
            }
        }

        struct cluster_t *clusters;
        int count = load_clusters(argv[1], &clusters);

        if (!clusters) {
            return EXIT_FAILURE;
        } else if (N > count) {
            fprintf(stderr, "ERROR: There is less objects then N!\n");
            return EXIT_FAILURE;
        }

        while (count > N) {
            int c1;
            int c2;

            find_neighbours(clusters, count, &c1, &c2);

            merge_clusters(&clusters[c1], &clusters[c2]);

            count = remove_cluster(clusters, count, c2);
        }

        print_clusters(clusters, count);

        for (int i = 0; i < count; ++i) {
            clear_cluster(&clusters[i]);
        }

        free(clusters);
    } else {
        fprintf(stderr, "ERROR: Bad arguments!");
        return EXIT_FAILURE;
    }

    return 0;
}