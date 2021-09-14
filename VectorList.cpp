#include <vector>
#include <list>

template<class T>
class VectorList
{
private:
    using VectT  = std::vector<T>;
    using ListT = std::list<VectT>;

public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const &) = default;
    VectorList(VectorList &&) noexcept  = default;

    VectorList & operator=(VectorList &&) noexcept    = default;
    VectorList & operator=(VectorList const &) = default;

    // метод, который будет использоваться для заполнения VectorList
    // гарантирует, что в списке не будет пустых массивов
    template<class It>
    void append(It p, It q) {
        if (p != q)
            data_.push_back(VectT(p,q));
    }

    bool empty() const { return size() == 0; }

    // определите метод size
    size_t size() const {
        size_t size = 0;
        for( auto& item: data_)
            size += item.size();
        return size;
    };


    class const_iterator: public std::iterator<std::bidirectional_iterator_tag, const T> {
    public:
        const_iterator(const VectorList* vList,typename VectT::const_iterator const& svIt,  typename ListT::const_iterator const& slIt):
                container_(&vList->data_), vIt(svIt), lIt(slIt) {};

        const_iterator() = default;
        const_iterator(const const_iterator &) = default;
        const_iterator( const_iterator &&) noexcept = default;


        const_iterator& operator=(const const_iterator& ) = default;
        const_iterator& operator=( const_iterator&& ) noexcept = default;

        bool operator!=(const_iterator const& other) const {
            return !(*this == other);
        };
        const_iterator operator++(int) { // проверить префиксный и постфиксный инкремент
            auto old = *this;
            ++(*this);
            return old;
        };

        const_iterator& operator++(){
            //cout << "++ operator" << endl;
            ++vIt;
            if ( vIt == lIt->end()) {
                ++lIt;
                if (lIt != container_->end()) vIt = lIt->begin();
            }
            return *this;
        }

        const_iterator operator--(int){
            auto old = *this;
            --(*this);
            return old;
        }

        const_iterator& operator--(){
            if(!container_->empty())  {
                if (lIt == container_->end() || vIt == lIt->begin()) {
                    if( lIt-- != container_->begin()) vIt = lIt->end();
                }
                --vIt;
            }

            return *this;
        }

        bool operator==(const const_iterator& other ) const {
            return  (vIt == other.vIt) && (lIt == other.lIt) && (container_ == other.container_);
        }

        typename const_iterator::reference  operator*()  const {
            return *vIt;
        }

        typename const_iterator::pointer operator->() const{
            return &*vIt;
        }

    private:
        typename VectT::const_iterator vIt;
        typename ListT::const_iterator lIt;
        const ListT* container_;

    };

    const_iterator begin() const {
        if (data_.size() == 0 ) return const_iterator();

        typename ListT::const_iterator lit = data_.begin();
        typename VectT::const_iterator vit = lit->begin();  // проверить(*)
        return const_iterator (this, vit, lit);
    }

    const_iterator end()   const {
        if(data_.size() == 0) return const_iterator();

        typename ListT::const_reverse_iterator lit = data_.rbegin();
        return const_iterator(this, lit->end(), data_.end());
    }

    // определить const_reverse_iterator
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // определить методы rbegin / ren
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()) ;   }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()) ; }


private:
    ListT data_;
};