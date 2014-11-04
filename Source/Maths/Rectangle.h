#if !defined RECTANGLE_INCLUDED
#define RECTANGLE_INCLUDED


/// <summary>
/// A basic Rectangle class, useful for representing spaces such as screen space.
/// </summary>
class Rectangle final
{
    public:
        
        #pragma region Constructors and destructor

        /// <summary> Construct a rectangle with a value for each co-ordinate. </summary>
        Rectangle (const int left, const int right, const int top, const int bottom);

        Rectangle()                                     = default;    
        Rectangle (const Rectangle& copy)               = default;
        Rectangle& operator= (const Rectangle& copy)    = default;
        ~Rectangle()                                    = default;

        Rectangle (Rectangle&& move);
        Rectangle& operator= (Rectangle&& move);

        #pragma endregion


        #pragma region Getters and setters

        int getLeft() const     { return m_left; }
        int getRight() const    { return m_right; }
        int getTop() const      { return m_top; }
        int getBottom() const   { return m_bottom; }

        /// <summary> Calculates and returns the width of the rectangle. </summary>
        int calcWidth() const   { return m_right - m_left + 1; }
        
        /// <summary> Calculates and returns the height of the rectangle. </summary>
        int calcHeight() const  { return m_bottom - m_top + 1; }

        void setLeft (const int left);
        void setRight (const int right);
        void setTop (const int top);
        void setBottom (const int bottom);

        #pragma endregion


        #pragma region Testing functionality
        
        bool operator== (const Rectangle& rhs) const;
        bool operator!= (const Rectangle& rhs) const;

        /// <summary> Checks if the given rectangle is contained within the current rectangle. </summary>
        bool contains (const Rectangle& other) const;

        /// <summary> Checks to see if any point on the given rectangle intersects with the current rectangle. </summary>
        bool intersects (const Rectangle& other) const;

        #pragma endregion


        #pragma region Manipulation functionality

        /// <summary></summary>
        void clipTo (const Rectangle& other);

        /// <summary> Translates each co-ordinate by the given amount. </summary>
        void translate (const int moveX, const int moveY);

        #pragma endregion

    private:
    
        #pragma region Member variables

        int m_left      { 0 },  //!< The left co-ordinate.
            m_right     { 0 },  //!< The right co-ordinate.
            m_top       { 0 },  //!< The top co-ordinate.
            m_bottom    { 0 };  //!< The bottom co-ordinate.

        #pragma endregion

};


#endif // RECTANGLE_INCLUDED