#ifndef _PAIRHEAP_H
#define _PAIRHEAP_H
 
#include<iostream>
#include<cstdlib>
#include<vector>
#include<utility>
using namespace std;
 
struct PairNode {
    int nodeindex;
    double element;
    PairNode *prev, *leftChild, *nextSibling;
 
     PairNode() {
    } PairNode(int i, double d, PairNode * p = NULL, PairNode * l =
           NULL, PairNode * n = NULL)
 :      nodeindex(i), element(d), prev(p), leftChild(l), nextSibling(n) {
    }
};
 
/**
 * 打印配对堆
 */
void printNode(PairNode * root)
{
    if (root == NULL)
        return;
    else {
        cout << root->nodeindex << "(" << root->element << ")" << "\t";
        cout << root->nodeindex << "'s next:";
        printNode(root->nextSibling);
        if (root->element < INT_MAX) {
            cout << root->nodeindex << "'s leftChild:";
            printNode(root->leftChild);
        }
    }
}
 
/**
 * 合并以first和second为根的两棵树
 * 函数开始时first->nextSibling必须为NULL
 * second可以为NULL
 * 树合并后，first成为新树的根节点
 */
void compareAndLink(PairNode * &first, PairNode * second)
{
    if (second == NULL)
        return;
    if (second->element < first->element) {    //谁小谁作父节点
        second->prev = first->prev;
        first->prev = second;
        first->nextSibling = second->leftChild;
        if (first->nextSibling != NULL)
            first->nextSibling->prev = first;
        second->leftChild = first;
        first = second;
    } else {
        second->prev = first;
        first->nextSibling = second->nextSibling;
        if (first->nextSibling != NULL)
            first->nextSibling->prev = first;
        second->nextSibling = first->leftChild;
        if (second->nextSibling != NULL)
            second->nextSibling->prev = second;
        first->leftChild = second;
    }
}
 
/**
 * 插入新项，返回指向新节点的指针
 */
PairNode *insert(PairNode * &root, PairNode * newNode)
{
    if (root == NULL)
        root = newNode;
    else
        compareAndLink(root, newNode);
    return newNode;
}
 
/**
 * 将指定的节点降低delta值
 */
void decreaseKey(PairNode * &root, PairNode * p, const double delta)
{
    if (delta <= 0) {
        cerr << "降低的值为非正数." << endl;
        exit(1);
    }
    p->element -= delta;
    if (p != root) {
        if (p->nextSibling != NULL)  //如果有右兄弟节点
            p->nextSibling->prev = p->prev;
        if (p->prev->leftChild == p)  //如果p是最左孩子
            p->prev->leftChild = p->nextSibling;
        else        //如果p不是最左孩子
            p->prev->nextSibling = p->nextSibling;
 
        p->nextSibling = NULL;
        compareAndLink(root, p);
    }
}
 
/**
 * 两趟归并firstSibling的所有右兄弟节点
 * firstSibling在函数开始时不为NULL，函数结束后成为新树的根节点
 */
PairNode *combineSiblings(PairNode * firstSibling)
{
    if (firstSibling->nextSibling == NULL)
        return firstSibling;
    static vector < PairNode * >treeArray(200);   //初始长度给定为200,需要时再用resize扩展
    int numSiblings = 0;
    for (; firstSibling != NULL; numSiblings++) {
        if (numSiblings == treeArray.size())
            treeArray.resize(numSiblings * 2);
        treeArray[numSiblings] = firstSibling;
        firstSibling->prev->nextSibling = NULL;   //虽然配对堆用的是双向链表存储节点，但只要打断向后的指针，使每个节点的nextSibling=NULL,再进行conpareAndLink时就不会出错
        firstSibling = firstSibling->nextSibling;
    }
    if (numSiblings == treeArray.size())
        treeArray.resize(numSiblings + 1);
    treeArray[numSiblings] = NULL;
    //第一趟，从左向右两两归并
    int i = 0;
    for (; i + 1 < numSiblings; i += 2)
        compareAndLink(treeArray[i], treeArray[i + 1]);
    int j = i - 2;
    if (j == numSiblings - 3)
        compareAndLink(treeArray[j], treeArray[j + 2]);
    //第二趟，从右向左逐个向前归并
    for (; j >= 2; j -= 2)
        compareAndLink(treeArray[j - 2], treeArray[j]);
    return treeArray[0];
}
 
/**
 * 移除最小节点（即根节点）
 */
void deleteMin(PairNode * &root)
{
    if (root == NULL) {
        cerr << "PairingHeap has already been empty,can't deleteMin." <<
            endl;
        exit(-1);
    }
    PairNode *oldRoot = root;
    if (root->leftChild == NULL)
        root = NULL;
    else
        root = combineSiblings(root->leftChild);
    delete oldRoot;     //释放空间
}
 
/**
 * 返回指定节点的指针
 */
PairNode *findNode(PairNode * &root, const pair < int, double >&node)
{
    if (root == NULL)
        return NULL;
    if (root->nodeindex == node.first)   //只要找到nodeindex匹配的就可以，因为只要nodeindex匹配，element就一定也匹配
        return root;
    if (node.second + 0.001 > root->element) {    //如果要找的节点node的element比root的element还小，则node不可能在以root为根的子树下面
        //按理来讲配对堆中任何一棵子树的根节点不会比子树中的其他节点大，由于计算误差的累积，这里我们给出0.001的容错量
        PairNode *rect = findNode(root->leftChild, node);    //先搜子节点
        if (rect != NULL) {
            return rect;
        } else {
            return findNode(root->nextSibling, node);    //再搜兄弟节点
        }
    } else {
        return findNode(root->nextSibling, node);
    }
}
 
/**
 * 释放配对堆
 */
void deleteHeap(PairNode * &root)
{
    if (root != NULL) {
        deleteHeap(root->leftChild);
        deleteHeap(root->nextSibling);
        delete root;
    }
}
 
#endif