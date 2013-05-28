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
 * ��ӡ��Զ�
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
 * �ϲ���first��secondΪ����������
 * ������ʼʱfirst->nextSibling����ΪNULL
 * second����ΪNULL
 * ���ϲ���first��Ϊ�����ĸ��ڵ�
 */
void compareAndLink(PairNode * &first, PairNode * second)
{
    if (second == NULL)
        return;
    if (second->element < first->element) {    //˭С˭�����ڵ�
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
 * �����������ָ���½ڵ��ָ��
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
 * ��ָ���Ľڵ㽵��deltaֵ
 */
void decreaseKey(PairNode * &root, PairNode * p, const double delta)
{
    if (delta <= 0) {
        cerr << "���͵�ֵΪ������." << endl;
        exit(1);
    }
    p->element -= delta;
    if (p != root) {
        if (p->nextSibling != NULL)  //��������ֵܽڵ�
            p->nextSibling->prev = p->prev;
        if (p->prev->leftChild == p)  //���p��������
            p->prev->leftChild = p->nextSibling;
        else        //���p����������
            p->prev->nextSibling = p->nextSibling;
 
        p->nextSibling = NULL;
        compareAndLink(root, p);
    }
}
 
/**
 * ���˹鲢firstSibling���������ֵܽڵ�
 * firstSibling�ں�����ʼʱ��ΪNULL�������������Ϊ�����ĸ��ڵ�
 */
PairNode *combineSiblings(PairNode * firstSibling)
{
    if (firstSibling->nextSibling == NULL)
        return firstSibling;
    static vector < PairNode * >treeArray(200);   //��ʼ���ȸ���Ϊ200,��Ҫʱ����resize��չ
    int numSiblings = 0;
    for (; firstSibling != NULL; numSiblings++) {
        if (numSiblings == treeArray.size())
            treeArray.resize(numSiblings * 2);
        treeArray[numSiblings] = firstSibling;
        firstSibling->prev->nextSibling = NULL;   //��Ȼ��Զ��õ���˫������洢�ڵ㣬��ֻҪ�������ָ�룬ʹÿ���ڵ��nextSibling=NULL,�ٽ���conpareAndLinkʱ�Ͳ������
        firstSibling = firstSibling->nextSibling;
    }
    if (numSiblings == treeArray.size())
        treeArray.resize(numSiblings + 1);
    treeArray[numSiblings] = NULL;
    //��һ�ˣ��������������鲢
    int i = 0;
    for (; i + 1 < numSiblings; i += 2)
        compareAndLink(treeArray[i], treeArray[i + 1]);
    int j = i - 2;
    if (j == numSiblings - 3)
        compareAndLink(treeArray[j], treeArray[j + 2]);
    //�ڶ��ˣ��������������ǰ�鲢
    for (; j >= 2; j -= 2)
        compareAndLink(treeArray[j - 2], treeArray[j]);
    return treeArray[0];
}
 
/**
 * �Ƴ���С�ڵ㣨�����ڵ㣩
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
    delete oldRoot;     //�ͷſռ�
}
 
/**
 * ����ָ���ڵ��ָ��
 */
PairNode *findNode(PairNode * &root, const pair < int, double >&node)
{
    if (root == NULL)
        return NULL;
    if (root->nodeindex == node.first)   //ֻҪ�ҵ�nodeindexƥ��ľͿ��ԣ���ΪֻҪnodeindexƥ�䣬element��һ��Ҳƥ��
        return root;
    if (node.second + 0.001 > root->element) {    //���Ҫ�ҵĽڵ�node��element��root��element��С����node����������rootΪ������������
        //����������Զ����κ�һ�������ĸ��ڵ㲻��������е������ڵ�����ڼ��������ۻ����������Ǹ���0.001���ݴ���
        PairNode *rect = findNode(root->leftChild, node);    //�����ӽڵ�
        if (rect != NULL) {
            return rect;
        } else {
            return findNode(root->nextSibling, node);    //�����ֵܽڵ�
        }
    } else {
        return findNode(root->nextSibling, node);
    }
}
 
/**
 * �ͷ���Զ�
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