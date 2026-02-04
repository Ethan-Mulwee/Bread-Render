#ifndef BRL_RENDER_DATA
#define BRL_RENDER_DATA

// temp for dynamic arrays make your own solution later
#include "brl_vertexbuffer.hpp"
#include "brl_color.hpp"

namespace brl {
    using namespace smath;
    
    enum RenderConfiguration {
        SOLID, WIREFRAME, TRANSPARENT
    };

    struct RenderData {
        RenderConfiguration config;
        Vertexbuffer vertexbuffer;
        matrix4x4 transform;
        Color color;
    };

    /* An automatically growing linear buffer that tracks how many of it's elements are valid/used
    I.e. the data can be 'cleared' each frame without any memory operations it is just makred as free.

    Unlike std::vector calling clear() does not call the destructor on the elements inside
    therefore saving any allocated memory buffers inside for the next frame just invalidating their data */
    template<typename T>
    struct ScratchBuffer {
        T* buffer;
        uint32_t used;
        uint32_t size;

        void resize(uint32_t new_size) {
            buffer = (T*)realloc(buffer, new_size * sizeof(T));
            size = new_size;
            if (used > size)
                used = size;
        }

        void add(T element) {
            if (used >= size)
                resize(size * 2);

                buffer[used] = element;
                used++;
        }

        void clear() {
            used = 0;
        }
        
        // shrink size to fit used
        void shrink();

    };

    /* Structure that also dynamically takes care of object data buffers on the OpenGL side. 
    The buffer sizes between OpenGL and here are in sync. When resizing glBufferData() is called to resize it. 
    This ensures the OpenGL buffer is only resized when needed rather than on every frame; 
    glBufferSubData() can be used instead */
    struct GLSyncBuffer {
        DynamicVertexbuffer vertexbuffer;
        InstanceData* databuffer;

        // set to true after resizing so we know that this buffer needs to resized at render time
        bool GLresize = false;
        uint32_t used;
        uint32_t size;

        void resize(uint32_t new_size) {
            databuffer = (InstanceData*)realloc(databuffer, new_size * sizeof(InstanceData));

            size = new_size;
            if (used > size)
                used = size;
        }

        void sync() {
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer.instanceVBO);
            if (GLresize)
                glBufferData(vertexbuffer.instanceVBO, size * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
            else {
                glBufferSubData(GL_ARRAY_BUFFER, 0, used * sizeof(InstanceData), databuffer);
            }
        }

        void add(const matrix4x4 &transform, const Color &color) {
            if (used >= size)
                resize(size * 2);

            databuffer[used] = InstanceData{transform, color.vector};
        }

        void clear() {
            used = 0;
        }

        // void shrink() {
            
        // }

    };

    struct ConfigurationRenderDataNode {
        RenderConfiguration config;
        ScratchBuffer<GLSyncBuffer> object_arrays;
    };


    /* 
    Used for what is essentially debug drawing, data in the dyanmic render tree is relevant only for one frame

    Memory management options:
        None - Leaves in the tree will only grow and never shrink optimizing for speed but memory will balloon over time unless explicitly shrunk
        Light - Leaves will track their their used elements across frames and determine whether or not they should shrink based on some heuristic
        Agressive - Same as light but memory will be freed more often under less strict conditions
        Always - Memory is always freed at the end of the frame

    Possible memory management heuristic:
    keep a running average of the amount of elements used every frame if the average_amount_used / size if drops below a threshold like 10% then shrink
    */
    struct DynamicRenderDataTree {

        std::vector<ConfigurationRenderDataNode> configurations;

        // Addes RenderData to the tree in it's corresponding leaf node i.e. ObjectRenderDataArray
        void add(const RenderData &data);

        // Called after each frame is rendered to invalidate all the data in the buffers of the tree without deallocating any memory
        // NOTE: check to see if clear() on a std::vector of a struct that contains more std::vector will deallocate any of the vectors which you don't want
        // it needs to be effiectlvey a recursive clear
        void clear();
    };

    /* 
    Also used for debug drawing but allows for higher efficiency with large batches of objects. 
    Instead of managing memory for objects internally the user provides a buffer of all the necessary data. 
    Allowing expensive memory shuffling can be handled by the user and possibly avoided
    */
    struct BatchRenderDataTree {

    };

    /* 
    Used for relatively static scene data. The SceneRenderDataTree Leaf only sends new data to openGL when a change has been made since the last frame.
    i.e. is meant to be GL_STATIC_DRAW. Its data also ins't invalidated at the end of the frame like Dyanmic and Batch it is persistent unless manually cleared.

    Update flags follow a recursive structure. When new data is added the SceneRenderDataTree's update flag is marked to true then the corresponding
    configuration branch has its update flag set along with the leaf. When it comes time to check for updates the tree's flag is checked then the update
    function recursively checks down the tree splitting at branches.

         U  - starts descent
        /|\
       U N U  - contiunes down left and right branch
      /| | |
     N U N U  - updates config 0 leaf 1 and config 2 leaf 0

    */
    struct SceneRenderDataTree {
        
    };
}

#endif