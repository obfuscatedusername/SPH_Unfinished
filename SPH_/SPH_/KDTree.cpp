/// Implementation derived from here: https://github.com/shawnchin/kd3

#include "KDTree.h"

#include <assert.h>
#include <string.h>

enum DIMS{
	xDim, 
	yDim, 
	zDim, 
	nDim
};

#define cmp(v1, v2) ((v1 > v2) ? 1 : ((v1 < v2) ? -1 :0))

static int cmpX(const void *a1, const void *a2){
	const struct dp *A1 = (const struct dp*)a1;
	const struct dp *A2 = (const struct dp*)a2;
	return cmp(A1->x, A2->x);
};

static int cmpY(const void *a1, const void *a2){
	const struct dp *A1 = (const struct dp*)a1;
	const struct dp *A2 = (const struct dp*)a2;
	return cmp(A1->y, A2->y);
};

static int cmpZ(const void *a1, const void *a2){
	const struct dp *A1 = (const struct dp*)a1;
	const struct dp *A2 = (const struct dp*)a2;
	return cmp(A1->z, A2->z);
};

//sorting iterator function
static int cmpSizeT(const void *a1, const void *a2){
	const size_t *A1 = (const size_t*)a1;
	const size_t *A2 = (const size_t*)a2;
	return cmp(*A1, *A2);
}

//compare function pointer data type
typedef int(*cmpFunc)(const void*, const void*);

//static array of cmp functions indexed by dimension
const cmpFunc funcSelect[] = {cmpX, cmpY, cmpZ};


#ifndef _DEBUG_MODE
inline static int isLeafNode(const struct node *node);
#else
#define isLeafNode(node) (node->left == NULL)
#endif

//KDTree::KDTree(std::vector<Particle>* pVec, size_t count, kdtree **pTree){
//	kdtree *tree = *pTree;
//	assert(count > 1); //just to make sure there's not NULL count
//	
//	//Reallocate the object if the pointer is NULL, or if count is not matching
//	if(!tree || tree->count != count){
//		//First, delete the previous tree object
//		if(tree) KDTreeDEL(&tree);
//		//Then allocate object and update the reference
//		tree = (kdtree*)malloc(sizeof(kdtree));
//		assert(tree != NULL);
//		*pTree = tree;
//
//		tree->count		= count;
//		tree->maxNodes	= ((count - 1) * 2) + 1;
//		tree->points	= (dp*)malloc(sizeof(struct dp) * count);
//		tree->nodeData  = (node*)malloc(sizeof(struct node) * tree->maxNodes);
//		assert(tree->points != NULL);
//		assert(tree->nodeData != NULL);
//	}
//
//	//reset control values
//	tree->nextNode = 0;
//	for(size_t i = 0; i < count; i ++){
//		tree->points[i].idx		= i;
//		Vec3f t					= pVec->at(i).GetPosition();
//		tree->points[i].x		= t.x;
//		tree->points[i].y		= t.y;
//		tree->points[i].z		= t.z;
//		tree->points[i].pIdx	= i;
//	}
//	tree->root = buildKDTree(0, count - 1, 0 , tree);
//}

KDTree::KDTree(){

}

kdtree* KDTree::BuildKDTree(std::vector<Particle>* pVec, size_t count, kdtree **pTree){	
	
	kdtree *tree = *pTree;
	assert(count > 1); //just to make sure there's not NULL count
	
	//Reallocate the object if the pointer is NULL, or if count is not matching
	if(!tree 
		|| tree->count != count){
		//First, delete the previous tree object
		if(tree) KDTreeDEL(&tree);
		//Then allocate object and update the reference
		tree = (kdtree*)malloc(sizeof(kdtree));
		assert(tree != NULL);
		*pTree = tree;

		tree->count		= count;
		tree->maxNodes	= ((count - 1) * 2) + 1;
		tree->points	= (dp*)malloc(sizeof(struct dp) * count);
		tree->nodeData  = (node*)malloc(sizeof(struct node) * tree->maxNodes);
		assert(tree->points != NULL);
		assert(tree->nodeData != NULL);
	}

	//reset control values
	tree->nextNode = 0;

	for(size_t i = 0; i < count; i ++){
		tree->points[i].idx		= i;
		Vec3f t					= pVec->at(i).GetPosition();
		tree->points[i].x		= t.x;
		tree->points[i].y		= t.y;
		tree->points[i].z		= t.z;
		tree->points[i].pIdx	= i;
	}
	tree->root = buildKDTree(0, count - 1, 0 , tree);
	return tree;
}

//search tree for points contained within the defined cube
void KDTree::KDTreeSearchSpace(kdtree *tree, kdIter **pIter, float xmin, float xmax, 
							   float ymin, float ymax, float zmin, float zmax){
	kdIter *iter = *pIter;
	struct space searchSpace;
	struct space domain; 

	assert (tree != NULL); //ensure we have an object
	assert (tree->root != NULL); //ensure it has > one node;
	assert (!isLeafNode(tree->root)); //ensure the root is not a leaf node

	//Create a new iterator, or, reset the existing one
	if(iter != NULL) 
		iterReset(iter);
	else{
		iter = iterNew();
		*pIter = iter; //write back new pointer to the object
	}

	//set the search space
	searchSpace.dim[xDim].min = xmin;
	searchSpace.dim[xDim].max = xmax;
	searchSpace.dim[yDim].min = ymin;
	searchSpace.dim[yDim].max = ymax;
	searchSpace.dim[zDim].min = zmin;
	searchSpace.dim[zDim].max = zmax;

	//set the initial search domain to infiniiiiittyyy
	domain.dim[xDim].min = -FLT_MAX;
	domain.dim[xDim].max =  FLT_MAX;
	domain.dim[yDim].min = -FLT_MAX;
	domain.dim[yDim].max =  FLT_MAX;
	domain.dim[zDim].min = -FLT_MAX;
	domain.dim[zDim].max =  FLT_MAX;

	//search the tree
	searchKDTree(tree, tree->root, 0, &searchSpace, &domain, iter);
}

//deallocate tree object and set pointer to NULL
void KDTree::KDTreeDEL(kdtree **pTree){
	kdtree *tree = *pTree;
	if(tree == NULL) return;
	else{
		free(tree->points);
		free(tree->nodeData);
		free(tree);
		*pTree = NULL;
	}
}

//returns the next entry in the iteration, or KDTREE_END if the end is reached
size_t KDTree::kdIterGetNext(kdIter *iter){
	if(iter->current == iter->size) 
		return KD_TREE_END;
	else return iter->data[iter->current++];
}

//rewinds the iterator
void KDTree::kdIterRewind(kdIter *iter){
	assert(iter != NULL);
	iter->current = 0;
}

//deallocate memory associated with the iterator
void KDTree::kdIterDEL(kdIter ** pIter){
	kdIter *iter = *pIter;
	if (iter == NULL) return;
	else{
		free(iter->data);
		free(iter);
		pIter = NULL;
	}
}

//sort entries within the iterator
void KDTree::kdIterSort(kdIter *iter){
	qsort(iter->data, iter->size, sizeof(size_t), cmpSizeT);
}


///internal ting

//return the next node
inline struct node* KDTree::nextNode(kdtree * tree){
	assert(tree->nextNode < tree->maxNodes);
	return &tree->nodeData[tree->nextNode++];
}

//return a branch node
inline struct node * KDTree::getBranchNode(kdtree *tree, double split){
	struct node *n = nextNode(tree);
	n->split = split;
	return n;
}

//return a leaf node, holds the index of the actual data point
inline struct node* KDTree::getLeafNode(kdtree *tree, size_t offset){
	struct node *n = nextNode(tree);
	n->left		= NULL;
	n->right	= NULL;
	n->idx		= offset;
	return n;
}

#ifndef _DEBUG_MODE
inline static int isLeafNode(const struct node *n){
	return((n->left == NULL) && (n->right == NULL));
}
#endif

//returns true if point is within search space
inline int KDTree::pointInSearchSpace(const struct dp *point, const struct space *ss){
	return	( (point->x <= ss->dim[xDim].max) && (point->x >= ss->dim[xDim].min) &&
			  (point->y <= ss->dim[yDim].max) && (point->y >= ss->dim[yDim].min) &&
			  (point->z <= ss->dim[zDim].max) && (point->z >= ss->dim[zDim].min) );
}

//returns true if the domain is completely enclosed within the search space
inline int KDTree::completelyEnclosed(const struct space *ss, const struct space *dom){
	return ( (dom->dim[xDim].min <= ss->dim[xDim].max) && (dom->dim[xDim].min >= ss->dim[xDim].min) &&
			 (dom->dim[xDim].max <= ss->dim[xDim].max) && (dom->dim[xDim].max >= ss->dim[xDim].min) && 
			 
			 (dom->dim[yDim].min <= ss->dim[yDim].max) && (dom->dim[yDim].min >= ss->dim[yDim].min) && 
			 (dom->dim[yDim].max <= ss->dim[yDim].max) && (dom->dim[yDim].max >= ss->dim[yDim].min) &&
			 
			 (dom->dim[zDim].min <= ss->dim[zDim].max) && (dom->dim[zDim].min >= ss->dim[zDim].min) &&
			 (dom->dim[zDim].max <= ss->dim[zDim].max) && (dom->dim[zDim].max >= ss->dim[zDim].min) );
}

//returns true if the search area intersects
inline int KDTree::searchAreaIntersect(const struct space *ss, const struct space *dom){
	return !( (ss->dim[xDim].min > dom->dim[xDim].max) || (ss->dim[xDim].max < dom->dim[xDim].min) ||
			  (ss->dim[yDim].min > dom->dim[yDim].max) || (ss->dim[yDim].max < dom->dim[yDim].min) ||
			  (ss->dim[zDim].min > dom->dim[zDim].max) || (ss->dim[zDim].max < dom->dim[zDim].min) );
}

//add all leaf nodes under a branch to the iterator
inline void KDTree::reportAllLeaves(const kdtree *tree, const struct node* n, kdIter *iter){
	if(isLeafNode(n))
		iterPush(iter, tree->points[n->idx].idx);
	else{
		reportAllLeaves(tree, n->left, iter);
		reportAllLeaves(tree, n->right, iter);
	}
}

//convenience function to explore a subdomain
inline void KDTree::exploreBranch(kdtree* tree, struct node *n, size_t depth, const struct space *ss, 
								  const struct space *dom, kdIter *iter){
	 if(isLeafNode(n)){
		if(pointInSearchSpace(tree->points + n->idx, ss)){
			iterPush(iter, tree->points[n->idx].idx);
		}
	 }else if (searchAreaIntersect(ss, dom)){
		 if(completelyEnclosed(ss, dom)){
			reportAllLeaves(tree, n, iter);
		 } else{
			 searchKDTree(tree, n, depth +1 , ss, dom, iter);
		 }
	 }
}

void KDTree::searchKDTree(kdtree *tree, struct node *n, size_t depth, const struct space *ss, 
						  const struct space *dom, kdIter *iter){
	const size_t axis = depth % nDim;
	struct space nDom;

	//init boundaries for new domain 
	memcpy(&nDom, dom, sizeof(struct space));

	//explore the left branch
	nDom.dim[axis].max = (float)n->split;
	exploreBranch(tree, n->left, depth, ss, &nDom, iter);

	//explore right branch
	nDom.dim[axis].max = dom->dim[axis].max; //reset
	nDom.dim[axis].min = (float)n->split;
	exploreBranch(tree, n->right, depth, ss, &nDom, iter);
}

struct node* KDTree::buildKDTree(size_t idxFrom, size_t idxTo, size_t depth, kdtree *tree){
	double split;
	struct node *node;
	struct dp	*dataPoint;
	const size_t count	= idxTo - idxFrom + 1;
	const size_t mid	= idxFrom + ((idxTo - idxFrom) / 2);
	const size_t axis	= depth % nDim;

	//if there is only one point, return a leaf node
	if (count == 1) return getLeafNode(tree, idxFrom);

	//sort points within group to determine median point
	qsort((void*)(tree->points + idxFrom), count, sizeof(struct dp), funcSelect[axis]);

	//determine point where axis will be split
	dataPoint = &tree->points[mid];
	split = (axis == 0) ? dataPoint->x : ((axis == 1) ? dataPoint-> y : dataPoint->z);

	//recursively build a tree for the left and right planes
	node = getBranchNode(tree, split);
	node->left = buildKDTree(idxFrom, mid, depth+1, tree);
	node->right = buildKDTree(mid + 1, idxTo, depth + 1, tree);

	return node;
}

//allocate a new iterator object
inline kdIter* KDTree::iterNew(void){
	kdIter *iter = (kdIter*)malloc(sizeof(kdIter));
	assert(iter != NULL);

	iter->current	= 0;
	iter->size		= 0;
	iter->capacity	= KD_ITERATOR_INIT_SIZE;
	iter->data = (size_t*)malloc(sizeof(size_t) * iter->capacity);
	assert(iter->data != NULL);
	return iter;
}

inline void KDTree::iterReset(kdIter *iter){
	assert(iter != NULL);
	iter->size		= 0; 
	iter->current	= 0;
}

//reset the iterator, resize if memory is full
inline void KDTree::iterPush(kdIter *iter, size_t value){
	if(iter->size == iter->capacity){
		assert(KD_ITERATOR_GR > 1.0);
		iter->capacity *= KD_ITERATOR_GR;
		iter->data = (size_t*)realloc(iter->data, sizeof(size_t) * iter->capacity);
	}
	iter->data[iter->size++] = value;
}