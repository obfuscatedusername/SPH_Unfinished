/// Implementation derived from here: https://github.com/shawnchin/kd3

#ifndef KDTree_h
#define KDTree_h
#include <stdint.h>
#include <stdlib.h>
#include "Particle.h"
//#include "ParticleSystem.h"
#include "Vec3f.h"
#include <vector>

#define KD_ITERATOR_INIT_SIZE 50
#define KD_ITERATOR_GR 2

#ifndef SIZE_MAX
	#define KD_TREE_END ((size_t)-1)
#else
	#define KD_TREE_END SIZE_MAX
#endif 

struct node{
	struct node *left;
	struct node *right;
	double split;
	size_t idx;
};

struct dp{
	float x, y, z;
	size_t idx; 
	size_t pIdx; //the particle id
};

struct kdtree{
	kdtree(){
		count = maxNodes = nextNode = 0;
		points = NULL;
		nodeData = NULL;
		root = NULL;
	}
	size_t count; 
	size_t maxNodes;
	size_t nextNode;
	struct dp *points;
	struct node *nodeData;
	struct node *root;
};

struct sSpace{
	float xMin, xMax, yMin, yMax, zMin, zMax;
};

struct kdIter{
	kdIter(){
		data = NULL;
		capacity = NULL;
		size = current = 0;
	}
	size_t *data;
	size_t capacity; 
	size_t size;
	size_t current; 
};

struct bounds{
	float min, max;
};

struct space{
	struct bounds dim[3];
};

class KDTree{
public:
	//KDTree(std::vector<Particle>*, size_t, kdtree **);
	
	KDTree();
	~KDTree();
	kdtree* BuildKDTree(std::vector<Particle>*, size_t, kdtree **);
	void KDTreeDEL(kdtree **tree);
	void KDTreeSearch(kdtree *, kdIter **, Particle *, double);
	void KDTreeSearchSpace(kdtree *, kdIter **, float, float, float, float, float, float);
	size_t kdIterGetNext(kdIter *);
	void kdIterRewind(kdIter *);
	void kdIterSort(kdIter *);
	void kdIterDEL(kdIter **);
	
	inline static struct node*	getLeafNode(kdtree *, size_t);
	inline static struct node*	getBranchNode(kdtree *, double);
	inline static kdIter*		iterNew(void);
private:
	inline static struct node*	nextNode(kdtree *tree);
	static struct node*			buildKDTree(size_t, size_t, size_t, kdtree *);
	
	inline static void			iterReset(kdIter *);
	inline static void			iterPush(kdIter *, size_t);
	inline static void			exploreBranch(kdtree *, struct node *, size_t, const struct space *, 
											  const struct space *, kdIter *);
	static void					searchKDTree(kdtree *, struct node *, size_t, const struct space *,
											 const struct space *, kdIter *);
	inline static int			pointInSearchSpace(const struct dp*, const struct space*);
	inline static int			completelyEnclosed(const struct space*, const struct space*);
	inline static int			searchAreaIntersect(const struct space*, const struct space*);
	inline static void			reportAllLeaves(const kdtree*, const struct node*, kdIter*);
};
#endif