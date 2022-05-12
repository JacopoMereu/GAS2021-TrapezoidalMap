#ifndef DAGNODE_H
#define DAGNODE_H

#include "cg3/geometry/point2.h"
#include "orderedsegment.h"
#include "drawables/drawabletrapezoid.h"


class DAGNode
{
public:
    /**
     * @brief The nodeType enum rapresents the type of the node, i.e. if it's a
     *      x-node
     *      y-node,
     *      or leaf node.
     */
    enum nodeType {x_node, y_node, leaf};


    /**
     * @brief The info union rapresents the type of information pointed by a node. A node can point to a:
     *      point,
     *      segment,
     *      or a trapezoid.
     *      The union allow us to save some space and it's suitable to our needs since only 1 of the 3 pointer will be stored in a given moment.
     */
    union info {
        const cg3::Point2d* p;
        const OrderedSegment* s;
        DrawableTrapezoid* t;
    };

    /////////// STATIC NODE GENERATORS: they generate a new node given the information to store in input ////////////////////////////
    /**
     * @brief generateXNode creates an x-node containing a pointer to a given point.
     * @param p         the pointer to the point to store inside the node.
     * @return          the pointer to new node created.
     */
    static DAGNode* generateXNode(const cg3::Point2d* const p);

    /**
     * @brief generateYNode creates a y-node containing a pointer to a given ordered segment.
     * @param s         the pointer to the orderedsegment to store inside the node.
     * @return          the pointer to the new node created.
     */
    static DAGNode* generateYNode(const OrderedSegment* const s);

    /**
     * @brief generateLeafNode creates a leaf containing a pointer to a given trapezoid.
     * If a node pointing to the trapezoid already exists, this last will be returned instead of creating a new one.
     * @param t                         the pointer to the trapezoid to store inside the node.
     * @param [out] leafAlreadyPresent  true  @if it a node pointing to "t" already exists
     *                                  false @else
     * @return                          the pointer to the node pointing to the trapezoid.
     */
    static DAGNode* generateLeafNode(DrawableTrapezoid* const t, bool& leafAlreadyPresent);

    /**
     * @brief newNode       Creates a new generic node given a type and a content in input
     * @param type          the type of the new node (x-node, y-node, leaf)
     * @param info          the information to store inside the node
     * @return              the new node
     */
    static DAGNode* newNode(nodeType type, info info);
    ///////////////////////////////////////////////////////////////////////////////////////////////////



    //////////////////////////// GETTERS ////////////////////////////////////////////////////////
    // get a reference to the type (x-node, y-node, leaf) of this node
    const nodeType& getNodeType() const;

    // return true if this node is a leaf, false otherwise
    bool isLeaf() const;

    // return true if this node is a x-node, false otherwise
    bool isXNode() const;

    // return true if this node is a y-node, false otherwise
    bool isYNode() const;

    // return the point pointed by this node if it's a x-node, nullptr otherwise
    const cg3::Point2d* getPointStored() const;

    // return the oriented segment pointed by this node if it's a y-node, nullptr otherwise
    const OrderedSegment* getOrientedSegmentStored() const;

    // return the (drawable) trapezoid pointed by this node if it's a leaf, nullptr otherwise
    DrawableTrapezoid* getTrapezoidStored() const;
    /////////////////////////////////////////////////////////////////////////////////////////////



    ////////////////////////////// CONVERTERS: they convert this node into another (e.g. from leaf to x-node) ////////////////////////////
    /**
     * @brief convertToXNode    converts this node into a x-node node.
     * @param p                 the pointer to the point to store inside the node.
     */
    void convertToXNode(const cg3::Point2d* const p);

    /**
     * @brief convertToYNode    converts this node into a y-node node.
     * @param s                 the pointer to the orderedsegment to store inside the node.
     */
    void convertToYNode(const OrderedSegment* const s);

    /**
     * @brief convertToLeafNode     converts this node into a leaf.
     * @param t                     the pointer to the (drawable) trapezoid to store inside the node.
     */
    void convertToLeafNode(DrawableTrapezoid* const t);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // pointer to the left child
    DAGNode* lc;
    // pointer to the right child
    DAGNode* rc;

private:
    // the type of this node
    nodeType type;
    // the (union containing the) value pointed by this node
    info value;
};

#endif // DAGNODE_H
