#if !defined RECTANGLE_INCLUDED
#define RECTANGLE_INCLUDED


/// <summary>
/// A basic Rectangle class, useful for representing spaces such as screen space.
/// </summary>
template <typename T> class Rectangle final
{
    public:
        
        #pragma region Constructors and destructor

        /// <summary> Construct a rectangle with a value for each co-ordinate. </summary>
        Rectangle (const T left, const T top, const T roght, const T bottom);

        Rectangle()                                     = default;
        Rectangle (const Rectangle& copy)               = default;
        Rectangle& operator= (const Rectangle& copy)    = default;
        ~Rectangle()                                    = default;

        Rectangle (Rectangle&& move);
        Rectangle& operator= (Rectangle&& move);

        #pragma endregion


        #pragma region Operators
        
        /// <summary> Returns a casted version of the rectangle with the desired type. </summary>
        template <typename U> operator Rectangle<U>() const;

        bool operator== (const Rectangle& rhs) const;
        bool operator!= (const Rectangle& rhs) const;

        #pragma endregion


        #pragma region Getters and setters

        T getLeft() const     { return m_left; }
        T getTop() const      { return m_top; }
        T getRight() const    { return m_right; }
        T getBottom() const   { return m_bottom; }

        void setLeft (const T left);
        void setTop (const T top);
        void setRight (const T right);
        void setBottom (const T bottom);

        #pragma endregion


        #pragma region Testing functionality

        /// <summary> Checks if the given rectangle is contained within the current rectangle. </summary>
        bool contains (const Rectangle& other) const;

        /// <summary> Checks to see if any point on the given rectangle intersects with the current rectangle. </summary>
        bool intersects (const Rectangle& other) const;

        #pragma endregion


        #pragma region Calculation functionality

        /// <summary> Calculates and returns the width of the rectangle. </summary>
        T width() const     { return m_right - m_left + 1; }
        
        /// <summary> Calculates and returns the height of the rectangle. </summary>
        T height() const    { return m_bottom - m_top + 1; }

        /// <summary> Calculates the area of the rectangle based on the calculated width and height values. </summary>
        T area() const      { return width() * height(); }

        /// <summary> Calculates the perimeter of the rectangle based on the calculated width and height values. </summary>
        T perimeter() const { return width() * 2 + height() * 2; }

        #pragma endregion


        #pragma region Manipulation functionality

        /// <summary> Returns a clipped version of the current rectangle which is inside the given rectangle. </summary>
        Rectangle clipped (const Rectangle& clip) const;

        /// <summary> Will clip the current rectangle so that it is inside of the given rectangle. </summary>
        void clipTo (const Rectangle& other);

        /// <summary> Translates each co-ordinate by the given amount. </summary>
        void translate (const T moveX, const T moveY);

        #pragma endregion

    private:
    
        #pragma region Member variables

        T   m_left      { (T) 0 },  //!< The left co-ordinate.
            m_top       { (T) 0 },  //!< The top co-ordinate.
            m_right     { (T) 0 },  //!< The right co-ordinate.
            m_bottom    { (T) 0 };  //!< The bottom co-ordinate.

        #pragma endregion

};


#endif // RECTANGLE_INCLUDED