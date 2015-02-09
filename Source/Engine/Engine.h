#if !defined WATER_ENGINE_INCLUDED
#define WATER_ENGINE_INCLUDED


// Engine namespace.
namespace water
{
    /// <summary> 
    /// A locator class used to access each engine subsystem from anywhere. 
    /// </summary>
    template <typename T> class Engine final
    {
        public:

            /// <summary> Obtain a reference to the current system. </summary>
            /// <returns> The system. </returns>
            static T* get()
            {
                return m_system;
            }


            /// <summary> Change the currently active system. </summary>
            /// <param name="system"> The desired system. </param>
            static void set (T* const system)
            {
                m_system = system;
            }
      
        private:

            static T* m_system; //!< A pointer to the current system.
    };

    template <typename T> typename T* Engine<T>::m_system = 0;
}

#endif // WATER_ENGINE_INCLUDED