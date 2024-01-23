#include "pch.h"
#include "chunk.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab3
{
    TEST_CLASS(Lab3)
    {
    public:

        TEST_METHOD(PushBackTest)
        {
            ChunkList<int, 5> chunkList;
            chunkList.push_back(1);
            chunkList.push_back(2);
            Assert::AreEqual(2, static_cast<int>(chunkList.len()));
        }

        TEST_METHOD(PushFrontTest)
        {
            ChunkList<int, 5> chunkList;
            chunkList.push_front(1);
            chunkList.push_front(2);
            Assert::AreEqual(2, static_cast<int>(chunkList.len()));
        }

    };
}