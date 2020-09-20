# env/avl_tree.h

## Macros

None.

## Types

#### `typedef struct __MCFCRT_tagAvlNodeHeader _MCFCRT_AvlNodeHeader;`

* **Description**

    This is an opaque `struct` representing an AVL node.  
    The user MUST treat the `struct` opaque and make no assumption of the bit representation of it.

#### `typedef struct __MCFCRT_tagAvlNodeHeader *_MCFCRT_AvlRoot;`

* **Description**

    This is the pointer type representing the root of an AVL tree. If the root pointer is not `NULL`, it points to a node roughly in the middle of the entire tree.  
    DO NOT write to the root pointer directly. To swap two AVL trees, use `_MCFCRT_AvlSwap()`.

#### `typedef int (*_MCFCRT_AvlComparatorNodes)(const _MCFCRT_AvlNodeHeader *, const _MCFCRT_AvlNodeHeader *);`
#### `typedef int (*_MCFCRT_AvlComparatorNodeOther)(const _MCFCRT_AvlNodeHeader *, intptr_t);`

* **Description**

    These are comparator callbacks for AVL trees to work. Either callback shall return a negative value, zero or a positive value if the left operand compares less than, equal to or greater than the right operand, respectively. The user shall ensure that the comparison of any two nodes using the first callback and the comparison of a node and a user-specified `intptr_t` using the second callback form the same [weak ordering](https://en.wikipedia.org/wiki/Weak_ordering). The behavior is undefined otherwise.

## Functions

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlPrev(const _MCFCRT_AvlNodeHeader *pNode);`

* **Description**

    This function obtains the inorder predecessor of the specified node.

* **Parameters**

    `const _MCFCRT_AvlNodeHeader *pNode`  
    The node whose inorder predecessor is to be obtained. This parameter must not be `NULL`.

* **Return Value**

    The return value is a pointer to the inorder predecessor, or `NULL` if there isn't.

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlNext(const _MCFCRT_AvlNodeHeader *pNode);`

* **Description**

    This function obtains the inorder successor of the specified node.

* **Parameters**

    `const _MCFCRT_AvlNodeHeader *pNode`  
    The node whose inorder successor is to be obtained. This parameter must not be `NULL`.

* **Return Value**

    The return value is a pointer to the inorder successor, or `NULL` if there isn't.

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlFront(const _MCFCRT_AvlRoot *ppRoot);`

* **Description**

    This function obtains the inorder first node of the entire tree.  

* **Parameters**

    `const _MCFCRT_AvlRoot *ppRoot`  
    The root of an AVL tree whose inorder first node is to be obtained. This parameter must not be `NULL`.

* **Return Value**

    The return value is a pointer to the inorder first node, or `NULL` if `*ppRoot` is `NULL`.

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlBack(const _MCFCRT_AvlRoot *ppRoot);`

* **Description**

    This function obtains the inorder last node of the entire tree.  

* **Parameters**

    `const _MCFCRT_AvlRoot *ppRoot`  
    The root of an AVL tree whose inorder last node is to be obtained. This parameter must not be `NULL`.

* **Return Value**

    The return value is a pointer to the inorder last node, or `NULL` if `*ppRoot` is `NULL`.

#### `void _MCFCRT_AvlSwap(_MCFCRT_AvlRoot *ppRoot1, _MCFCRT_AvlRoot *ppRoot2);`

* **Description**

    This function swaps two AVL trees.  

* **Parameters**

    `_MCFCRT_AvlRoot *ppRoot1` and `_MCFCRT_AvlRoot *ppRoot2`  
    The roots of two trees to swap. Both parameters must not be `NULL`.

* **Return Value**

    This function does not return a value.

* **Remarks**

    This is the only guaranteed way to swap two AVL trees.  

#### `void _MCFCRT_AvlAttachWithHint(_MCFCRT_AvlRoot *ppRoot, const _MCFCRT_AvlNodeHeader *pHint, _MCFCRT_AvlNodeHeader *pNode, _MCFCRT_AvlComparatorNodes pfnComparator);`

* **Description**

    This function inserts a node into the specified AVL tree according to the ordering defined by `pfnComparator`.  
    If any nodes that compare equal with `pNode` already exist in the AVL tree, `pNode` shall be inserted inorder after all those nodes.  
    If `pHint` is not `NULL` and `pNode` can be inserted right before or after `pHint` without breaking the above constraints,  the time complexity shall be O(1). Otherwise the time complexity shall be O(log(N)). Such behavior is compatible with both C++98 and C++11.  
    If the ordering defined by `pfnComparator` is not a strict weak ordering, the behavior is undefined.

* **Parameters**

    `_MCFCRT_AvlRoot *ppRoot`  
    The AVL tree to insert `pNode` into. This parameter must not be `NULL`.  

    `const _MCFCRT_AvlNodeHeader *pHint`  
    An optional hint for performance optimization. This parameter can be `NULL`.  

    `_MCFCRT_AvlNodeHeader *pNode`  
    The node to insert.  

    `_MCFCRT_AvlComparatorNodes pfnComparator`  
    A user-defined callback used to compare two nodes. This parameter must not be `NULL`.

* **Return Value**

    This function does not return a value.

* **Remarks**

    This function performs no memory management. It is the caller that should allocate the storage for the node.

#### `void _MCFCRT_AvlAttach(_MCFCRT_AvlRoot *ppRoot, _MCFCRT_AvlNodeHeader *pNode, _MCFCRT_AvlComparatorNodes pfnComparator);`

* **Description**

    This function is equivalent to `_MCFCRT_AvlAttachWithHint()` with the `pHint` parameter set to `NULL`.

* **Parameters**

    `_MCFCRT_AvlRoot *ppRoot`  
    The AVL tree to insert `pNode` into. This parameter must not be `NULL`.  

    `_MCFCRT_AvlNodeHeader *pNode`  
    The node to insert.  

    `_MCFCRT_AvlComparatorNodes pfnComparator`  
    A user-defined callback used to compare two nodes. This parameter must not be `NULL`.

* **Return Value**

    This function does not return a value.

#### `void _MCFCRT_AvlDetach(const _MCFCRT_AvlNodeHeader *pNode);`

* **Description**

    This function dissociates a node from the AVL tree that it has been inserted into.  
    If the node has not been inserted into or has already been detached from an AVL tree, the behavior is undefined.  

* **Parameters**

    `const _MCFCRT_AvlNodeHeader *pNode`  
    The node to dissociate.  

* **Return Value**

    This function does not return a value.

* **Remarks**

    This function performs no memory management. It is the caller that should deallocate the storage of the node.

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlGetLowerBound(const _MCFCRT_AvlRoot *ppRoot, intptr_t nOther, _MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther);`

* **Description**

    This function returns a pointer to the inorder first node, `pNode`, under the condition that `(*pfnComparatorNodeOther)(pNode, nOther) >= 0`. That is, for each node `pOther` inorder before `pNode` the assertion `(*pfnComparatorNodeOther)(pOther, nOther) < 0` holds and for each node `pOther` inorder after `pNode` as well as `pNode` itself the assertion `(*pfnComparatorNodeOther)(pOther, nOther) >= 0` holds.  
    If the ordering defined by `pfnComparatorNodeOther` is not a strict weak ordering, the behavior is undefined.

* **Parameters**

    `_MCFCRT_AvlRoot *ppRoot`  
    The AVL tree to search for nodes. This parameter must not be `NULL`.  

    `intptr_t nOther`  
    The comparand used by the comparator callback.  

    `_MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther`  
    A user-defined callback used to compare a node with an `intptr_t`. This parameter must not be `NULL`.

* **Return Value**

    This function returns a pointer to a node, or `NULL` if there isn't.

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlGetUpperBound(const _MCFCRT_AvlRoot *ppRoot, intptr_t nOther, _MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther);`

* **Description**

    This function returns a pointer to the inorder first node, `pNode`, under the condition that `(*pfnComparatorNodeOther)(pNode, nOther) > 0`. That is, for each node `pOther` inorder before `pNode` the assertion `(*pfnComparatorNodeOther)(pOther, nOther) <= 0` holds and for each node `pOther` inorder after `pNode` as well as `pNode` itself the assertion `(*pfnComparatorNodeOther)(pOther, nOther) > 0` holds.  
    If the ordering defined by `pfnComparatorNodeOther` is not a strict weak ordering, the behavior is undefined.

* **Parameters**

    `_MCFCRT_AvlRoot *ppRoot`  
    The AVL tree to search for nodes. This parameter must not be `NULL`.  

    `intptr_t nOther`  
    The comparand used by the comparator callback.  

    `_MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther`  
    A user-defined callback used to compare a node with an `intptr_t`. This parameter must not be `NULL`.

* **Return Value**

    This function returns a pointer to a node, or `NULL` if there isn't.

#### `_MCFCRT_AvlNodeHeader *_MCFCRT_AvlFind(const _MCFCRT_AvlRoot *ppRoot, intptr_t nOther, _MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther);`

* **Description**

    This function returns a pointer to the highest node, `pNode`, under the condition that `(*pfnComparatorNodeOther)(pNode, nOther) == 0`. That is, for each node `pOther` inorder before `pNode` the assertion `(*pfnComparatorNodeOther)(pOther, nOther) <= 0` holds and for each node `pOther` inorder after `pNode` the assertion `(*pfnComparatorNodeOther)(pOther, nOther) >= 0` holds.  
    If the ordering defined by `pfnComparatorNodeOther` is not a strict weak ordering, the behavior is undefined.

* **Parameters**

    `_MCFCRT_AvlRoot *ppRoot`  
    The AVL tree to search for nodes. This parameter must not be `NULL`.  

    `intptr_t nOther`  
    The comparand used by the comparator callback.  

    `_MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther`  
    A user-defined callback used to compare a node with an `intptr_t`. This parameter must not be `NULL`.

* **Return Value**

    This function returns a pointer to a node, or `NULL` if there isn't.

#### `void _MCFCRT_AvlGetEqualRange(_MCFCRT_AvlNodeHeader **restrict ppLower, _MCFCRT_AvlNodeHeader **restrict ppUpper, const _MCFCRT_AvlRoot *ppRoot, intptr_t nOther, _MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther);`

* **Description**

    This function is an efficient combination of the following two lines:

    ```c
*ppLower = _MCFCRT_AvlGetLowerBound(ppRoot, nOther, pfnComparatorNodeOther);
*ppUpper = _MCFCRT_AvlGetUpperBound(ppRoot, nOther, pfnComparatorNodeOther);
    ```

* **Parameters**

    `_MCFCRT_AvlNodeHeader **restrict ppLower`  
    The location where a pointer to the lower bound node will be written. This parameter must not be `NULL`.  

    `_MCFCRT_AvlNodeHeader **restrict ppUpper`  
    The location where a pointer to the upper bound node will be written. This parameter must not be `NULL`.  

    `_MCFCRT_AvlRoot *ppRoot`  
    The AVL tree to search for nodes. This parameter must not be `NULL`.  

    `intptr_t nOther`  
    The comparand used by the comparator callback.  

    `_MCFCRT_AvlComparatorNodeOther pfnComparatorNodeOther`  
    A user-defined callback used to compare a node with an `intptr_t`. This parameter must not be `NULL`.

* **Return Value**

    This function does not return a value.
