#include "pch.h"
#include "CppUnitTest.h"
#include "HT.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HTtests
{
	TEST_CLASS(HTtests)
	{
	public:
		
		BEGIN_TEST_METHOD_ATTRIBUTE(TwoHT)
			TEST_PRIORITY(0)
		END_TEST_METHOD_ATTRIBUTE()

		BEGIN_TEST_METHOD_ATTRIBUTE(InsertMax)
			TEST_PRIORITY(1)
		END_TEST_METHOD_ATTRIBUTE()

		BEGIN_TEST_METHOD_ATTRIBUTE(InsertDuplicate)
			TEST_PRIORITY(2)
		END_TEST_METHOD_ATTRIBUTE()

		BEGIN_TEST_METHOD_ATTRIBUTE(DeleteAll)
			TEST_PRIORITY(3)
		END_TEST_METHOD_ATTRIBUTE()

		BEGIN_TEST_METHOD_ATTRIBUTE(InsertOverFill)
			TEST_PRIORITY(4)
		END_TEST_METHOD_ATTRIBUTE()

		BEGIN_TEST_METHOD_ATTRIBUTE(GetNotExisting)
			TEST_PRIORITY(5)
		END_TEST_METHOD_ATTRIBUTE()


		TEST_METHOD(TwoHT)
		{
			const char* filename1 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\FirstHT.txt";
			const char* filename2 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\SecondHT.txt";
			HT::HTHANDLE* FirstHT = HT::Create(100, 3000, 10, 226, filename1);
			HT::HTHANDLE* SecondHT = HT::Create(100, 3000, 10, 226, filename2);
			FirstHT = HT::Open(filename1);
			SecondHT = HT::Open(filename2);
			for (int i = 0; i < 5; i++)
			{
				int key = i;
				int key2 = i + 5;
				int value = i * 100;
				int value2 = (i + 5) * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Element* newElement2 = new HT::Element(&key2, sizeof(key), &value2, sizeof(value2));
				HT::Insert(FirstHT, newElement);
				HT::Insert(SecondHT, newElement2);
				HT::Element* getElement1 = new HT::Element(&key, sizeof(key));
				HT::Element* getElement2 = new HT::Element(&key2, sizeof(key2));
				HT::Element* gotEl = HT::Get(FirstHT, getElement1);
				HT::Element* gotEl2 =HT::Get(SecondHT, getElement2);
				HT::print(gotEl);
				HT::print(gotEl2);
			}
			HT::Close(FirstHT);
			HT::Close(SecondHT);
		}

		TEST_METHOD(InsertMax)
		{
			const char* filename1 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\FirstHT.txt";
			HT::HTHANDLE* FirstHT = HT::Open(filename1);
			for (int i = 0; i < 100; i++)
			{
				int key = i;
				int value = i * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Insert(FirstHT, newElement);
			}
			int expectedI = 97;
			int expectedV = 97 * 100;
			HT::Element* expected = new HT::Element(&expectedI, sizeof(expectedI), &expectedV, sizeof(expectedV));

			HT::Element* gotEl = HT::Get(FirstHT, new HT::Element(&expectedI, sizeof(expectedI)));
			int value = *(int*)gotEl->payload;
			HT::Close(FirstHT);
			Assert::AreEqual(expectedV, value);
		}

		TEST_METHOD(InsertDuplicate)
		{
			const char* filename1 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\FirstHT.txt";
			const char* duplicate = "Key exists";
			const char* current;
			HT::HTHANDLE* FirstHT = HT::Open(filename1);
			for (int i = 0; i < 5; i++)
			{
				int key = i;
				int value = i * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Insert(FirstHT, newElement);
			}
			current = FirstHT->LastErrorMessage;
			HT::Close(FirstHT);
			Assert::AreEqual(duplicate, current);
		}
		
		TEST_METHOD(DeleteAll)
		{
			const char* filename1 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\FirstHT.txt";
			HT::HTHANDLE* FirstHT = HT::Open(filename1);
			for (int i = 0; i < 100; i++)
			{
				int key = i;
				int value = i * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Delete(FirstHT, newElement);
			}
			HT::Close(FirstHT);
		}

		TEST_METHOD(InsertOverFill)
		{
			const char* filename1 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\FirstHT.txt";
			const char* duplicate = "Full";
			const char* current;
			HT::HTHANDLE* FirstHT = HT::Open(filename1);
			for (int i = 0; i < 200; i++)
			{
				int key = i;
				int value = i * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Insert(FirstHT, newElement);
			}
			current = FirstHT->LastErrorMessage;
			HT::Close(FirstHT);
			Assert::AreEqual(duplicate, current);
		}

		TEST_METHOD(GetNotExisting)
		{
			const char* filename2 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\SecondHT.txt";
			const char* duplicate = "No such key";
			const char* current;
			HT::HTHANDLE* SecondHT = HT::Open(filename2);
			int key = 50;
			HT::Element* getElement = new HT::Element(&key, sizeof(key));
			HT::Element* gotEl2 = HT::Get(SecondHT, getElement);
			current = SecondHT->LastErrorMessage;
			HT::Close(SecondHT);
			Assert::AreEqual(duplicate, current);
		}

		TEST_METHOD(DeleteNotExisting)
		{
			const char* filename1 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\FirstHT.txt";
			const char* duplicate = "There is no such element to delete";
			const char* current;
			HT::HTHANDLE* FirstHT = HT::Open(filename1);
			for (int i = 100; i < 101; i++)
			{
				int key = i;
				int value = i * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Delete(FirstHT, newElement);
			}
			current = FirstHT->LastErrorMessage;
			HT::Close(FirstHT);
			Assert::AreEqual(duplicate, current);
		}

		TEST_METHOD(OverloadKey)
		{
			const char* filename2 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\SecondHT.txt";
			HT::HTHANDLE* FirstHT = HT::Open(filename2);

				int key = 1;
				int value = 1 * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Insert(FirstHT, newElement);

			int expectedI = -46;
			int expectedV = -46 * 100;
			HT::Element* expected = new HT::Element(&expectedI, sizeof(expectedI), &expectedV, sizeof(expectedV));

			HT::Element* gotEl = HT::Get(FirstHT, new HT::Element(&expectedI, sizeof(expectedI)));
			int value = *(int*)gotEl->payload;
			HT::Close(FirstHT);
			Assert::AreEqual(-45000, value);
		}

		TEST_METHOD(InsertMinus)
		{
			const char* filename2 = "C:\\Users\\kalos\\Documents\\3kurs\\OS\\lab\\lab2\\SecondHT.txt";
			HT::HTHANDLE* FirstHT = HT::Open(filename2);
			for (int i = 0; i > -50; i--)
			{
				int key = i;
				int value = i * 100;
				HT::Element* newElement = new HT::Element(&key, sizeof(key), &value, sizeof(value));
				HT::Insert(FirstHT, newElement);
			}
			int expectedI = -46;
			int expectedV = -46 * 100;
			HT::Element* expected = new HT::Element(&expectedI, sizeof(expectedI), &expectedV, sizeof(expectedV));

			HT::Element* gotEl = HT::Get(FirstHT, new HT::Element(&expectedI, sizeof(expectedI)));
			int value = *(int*)gotEl->payload;
			HT::Close(FirstHT);
			Assert::AreEqual(-45000, value);
		}
	};
}
