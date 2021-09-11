#pragma once
template <typename I>
struct BaseNode {
    I item;
    virtual ~BaseNode() {}
protected:
    BaseNode(I&& i) : item{ i } {}

    template <class N>
    static N* Allocate(I&& i) { return new N{ std::move(i) }; }
};

template <typename I>
struct DirectedNode : BaseNode<I> {
    DirectedNode(I&& i)
        : BaseNode<I>(std::forward<I>(i)), next{} {}
    DirectedNode(DirectedNode&& dn) noexcept
        : BaseNode<I>(std::forward<I>(dn.item)), next{ dn.next } {
        dn.item = I{};
        dn.next = nullptr;
    }
    ~DirectedNode() override {}

    DirectedNode* next;
};

template <typename I>
struct BiDirectionalNode : BaseNode<I> {
    BiDirectionalNode(I&& i)
        : BaseNode<I>(std::forward<I>(i)), next{}, prev{} {}
    ~BiDirectionalNode() override {}

    BiDirectionalNode* next;
    BiDirectionalNode* prev;
};

template <template <typename> class N, typename I>
struct HNode {
    using Node = N<I>;
    HNode(Node* n) : node{ n } {};
    HNode(HNode&& hn) : node{ hn.Release() } {}
    ~HNode() { delete node; node = nullptr; }
    Node& operator*() { return *node; }
    Node* operator->() { return node; }
    Node* Release() { Node* n{ node }; node = nullptr; return n; }
    Node* node;
};

template <template <typename> class N, typename I>
struct Acquire : N<I> {
    using Node = N<I>;
    static HNode<N, I> Instance(I&& i) { return HNode<N, I>{ typename Node::template Allocate<Node>(std::forward<I>(i)) }; }
};