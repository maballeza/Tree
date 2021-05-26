template<typename I>
struct BaseNode {
    I item;
    virtual ~BaseNode() {}
protected:
    BaseNode(I&& t) : item{ t } {}
};

template<typename I>
struct DirectedNode : BaseNode<I> {
    DirectedNode(I&& t) :
        BaseNode<I>(std::forward<I>(t)), next{} {}
    DirectedNode(DirectedNode&& bdn)
        : BaseNode<I>(std::forward<I>(bdn.item)), next{ bdn.next } {
        bdn.item = I{};
        bdn.next = nullptr;
    }
    ~DirectedNode() override {}

    DirectedNode* next;
};

template<typename I>
struct BiDirectionalNode : BaseNode<I> {
    BiDirectionalNode(I&& t) : BaseNode<I>(std::forward<I>(t)), next{}, prev{} {}
    ~BiDirectionalNode() override {}
    
    BiDirectionalNode* next;
    BiDirectionalNode* prev;
};