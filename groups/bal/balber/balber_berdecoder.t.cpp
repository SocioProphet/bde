// balber_berdecoder.t.cpp                                            -*-C++-*-

// ----------------------------------------------------------------------------
//                                   NOTICE
//
// This component is not up to date with current BDE coding standards, and
// should not be used as an example for new development.
// ----------------------------------------------------------------------------


#include <balber_berdecoder.h>

#include <balber_berencoder.h>        // for testing only

#include <bdlat_attributeinfo.h>
#include <bdlat_selectioninfo.h>
#include <bdlat_valuetypefunctions.h>

#include <bslim_testutil.h>

#include <bdlsb_memoutstreambuf.h>      // for testing only
#include <bdlsb_fixedmeminstreambuf.h>  // for testing only

#include <bslma_allocator.h>

#include <bsls_objectbuffer.h>
#include <bsls_review.h>
#include <bsls_stopwatch.h>
#include <bsls_types.h>

#include <bdlt_date.h>
#include <bdlt_datetime.h>
#include <bdlt_prolepticdateimputil.h>
#include <bdlt_time.h>

#include <bdlb_chartype.h>
#include <bdlb_print.h>
#include <bdlb_printmethods.h>

#include <bslim_testutil.h>

#include <bslma_testallocator.h>
#include <bslma_testallocatormonitor.h>

#include <bsl_cstdlib.h>
#include <bsl_fstream.h>
#include <bsl_iostream.h>
#include <bsl_iomanip.h>

using namespace BloombergLP;
using namespace bsl;  // automatically added by script

// ============================================================================
//                             TEST PLAN
// ----------------------------------------------------------------------------

// ============================================================================
//                      STANDARD BDE ASSERT TEST MACRO
// ----------------------------------------------------------------------------
static int testStatus = 0;

static void aSsErT(int c, const char *s, int i) {
    if (c) {
        bsl::cout << "Error " << __FILE__ << "(" << i << "): " << s
                  << "    (failed)" << bsl::endl;
        if (testStatus >= 0 && testStatus <= 100) ++testStatus;
    }
}

// ============================================================================
//               STANDARD BDE TEST DRIVER MACRO ABBREVIATIONS
// ----------------------------------------------------------------------------

#define ASSERT       BSLIM_TESTUTIL_ASSERT
#define ASSERTV      BSLIM_TESTUTIL_ASSERTV

#define LOOP_ASSERT  BSLIM_TESTUTIL_LOOP_ASSERT
#define LOOP0_ASSERT BSLIM_TESTUTIL_LOOP0_ASSERT
#define LOOP1_ASSERT BSLIM_TESTUTIL_LOOP1_ASSERT
#define LOOP2_ASSERT BSLIM_TESTUTIL_LOOP2_ASSERT
#define LOOP3_ASSERT BSLIM_TESTUTIL_LOOP3_ASSERT
#define LOOP4_ASSERT BSLIM_TESTUTIL_LOOP4_ASSERT
#define LOOP5_ASSERT BSLIM_TESTUTIL_LOOP5_ASSERT
#define LOOP6_ASSERT BSLIM_TESTUTIL_LOOP6_ASSERT

#define ASSERT_EQ(X,Y) BSLIM_TESTUTIL_LOOP2_ASSERT(X,Y,X == Y)
#define ASSERT_NE(X,Y) BSLIM_TESTUTIL_LOOP2_ASSERT(X,Y,X != Y)

#define ASSERT1_EQ(L,X,Y) BSLIM_TESTUTIL_LOOP3_ASSERT(L,X,Y,X == Y)
#define ASSERT1_NE(L,X,Y) BSLIM_TESTUTIL_LOOP3_ASSERT(L,X,Y,X != Y)

#define Q            BSLIM_TESTUTIL_Q   // Quote identifier literally.
#define P            BSLIM_TESTUTIL_P   // Print identifier and value.
#define P_           BSLIM_TESTUTIL_P_  // P(X) without '\n'.
#define T_           BSLIM_TESTUTIL_T_  // Print a tab (w/o newline).
#define L_           BSLIM_TESTUTIL_L_  // current Line number

// ============================================================================
//                   GLOBAL TYPEDEFS/CONSTANTS FOR TESTING
// ----------------------------------------------------------------------------

static bool         verbose = false;
static bool     veryVerbose = false;
static bool veryVeryVerbose = false;

// ============================================================================
//                    GLOBAL HELPER FUNCTIONS FOR TESTING
// ----------------------------------------------------------------------------

bsl::vector<char> loadFromHex(const char *hexData)
{
    char firstDigit = 0;

    bsl::vector<char> result;

    char hexValueTable[128] =
    {
        0 , //   0   0
        0 , //   1   1
        0 , //   2   2
        0 , //   3   3
        0 , //   4   4
        0 , //   5   5
        0 , //   6   6
        0 , //   7   7
        0 , //   8   8 - BACKSPACE
        0 , //   9   9 - TAB
        0 , //  10   a - LF
        0 , //  11   b
        0 , //  12   c
        0 , //  13   d - CR
        0 , //  14   e
        0 , //  15   f
        0 , //  16  10
        0 , //  17  11
        0 , //  18  12
        0 , //  19  13
        0 , //  20  14
        0 , //  21  15
        0 , //  22  16
        0 , //  23  17
        0 , //  24  18
        0 , //  25  19
        0 , //  26  1a
        0 , //  27  1b
        0 , //  28  1c
        0 , //  29  1d
        0 , //  30  1e
        0 , //  31  1f
        0 , //  32  20 - SPACE
        0 , //  33  21 - !
        0 , //  34  22 - "
        0 , //  35  23 - #
        0 , //  36  24 - $
        0 , //  37  25 - %
        0 , //  38  26 - &
        0 , //  39  27 - '
        0 , //  40  28 - (
        0 , //  41  29 - )
        0 , //  42  2a - *
        0 , //  43  2b - +
        0 , //  44  2c - ,
        0 , //  45  2d - -
        0 , //  46  2e - .
        0 , //  47  2f - /
        0 , //  48  30 - 0
        1 , //  49  31 - 1
        2 , //  50  32 - 2
        3 , //  51  33 - 3
        4 , //  52  34 - 4
        5 , //  53  35 - 5
        6 , //  54  36 - 6
        7 , //  55  37 - 7
        8 , //  56  38 - 8
        9 , //  57  39 - 9
        0 , //  58  3a - :
        0 , //  59  3b - ;
        0 , //  60  3c - <
        0 , //  61  3d - =
        0 , //  62  3e - >
        0 , //  63  3f - ?
        0 , //  64  40 - @
        10 , //  65  41 - A
        11 , //  66  42 - B
        12 , //  67  43 - C
        13 , //  68  44 - D
        14 , //  69  45 - E
        15 , //  70  46 - F
        0 , //  71  47 - G
        0 , //  72  48 - H
        0 , //  73  49 - I
        0 , //  74  4a - J
        0 , //  75  4b - K
        0 , //  76  4c - L
        0 , //  77  4d - M
        0 , //  78  4e - N
        0 , //  79  4f - O
        0 , //  80  50 - P
        0 , //  81  51 - Q
        0 , //  82  52 - R
        0 , //  83  53 - S
        0 , //  84  54 - T
        0 , //  85  55 - U
        0 , //  86  56 - V
        0 , //  87  57 - W
        0 , //  88  58 - X
        0 , //  89  59 - Y
        0 , //  90  5a - Z
        0 , //  91  5b - [
        0 , //  92  5c - '\'
        0 , //  93  5d - ]
        0 , //  94  5e - ^
        0 , //  95  5f - _
        0 , //  96  60 - `
        10 , //  97  61 - a
        11 , //  98  62 - b
        12 , //  99  63 - c
        13 , // 100  64 - d
        14 , // 101  65 - e
        15 , // 102  66 - f
        0 , // 103  67 - g
        0 , // 104  68 - h
        0 , // 105  69 - i
        0 , // 106  6a - j
        0 , // 107  6b - k
        0 , // 108  6c - l
        0 , // 109  6d - m
        0 , // 110  6e - n
        0 , // 111  6f - o
        0 , // 112  70 - p
        0 , // 113  71 - q
        0 , // 114  72 - r
        0 , // 115  73 - s
        0 , // 116  74 - t
        0 , // 117  75 - u
        0 , // 118  76 - v
        0 , // 119  77 - w
        0 , // 120  78 - x
        0 , // 121  79 - y
        0 , // 122  7a - z
        0 , // 123  7b - {
        0 , // 124  7c - |
        0 , // 125  7d - }
        0 , // 126  7e - ~
        0   // 127  7f - DEL
    };

    while (*hexData) {
        if (' ' == *hexData) {
            ++hexData;
            continue;
        }

        if (0 == firstDigit) {
            firstDigit = *hexData;
        }
        else {
            char value = static_cast<char>(  (hexValueTable[firstDigit] << 4)
                                           | hexValueTable[*hexData]);

            result.push_back(value);
            firstDigit = 0;
        }

        ++hexData;
    }

    ASSERT(0 == firstDigit);

    return result;
}

void printBuffer(const char *buffer, bsl::size_t length)
    // Print the specified 'buffer' of the specified 'length' in hex form
{
    cout << hex;
    int numOutput = 0;
    for (bsl::size_t i = 0; i < length; ++i) {
        if ((unsigned char) buffer[i] < 16) {
            cout << '0';
        }
        cout << (int) (unsigned char) buffer[i];
        numOutput += 2;
        if (0 == numOutput % 8) {
            cout << " ";
        }
    }
    cout << dec << endl;
}

void printDiagnostic(balber::BerDecoder & decoder)
{
    if (veryVerbose) {
        bsl::cout << decoder.loggedMessages();
    }
}

// ============================================================================
//                     GLOBAL HELPER CLASSES FOR TESTING
// ----------------------------------------------------------------------------

// BDE_VERIFY pragma: push
// BDE_VERIFY pragma: -MR01

// The code below was generated using the following command:
//..
// /opt/bb/bin/bas_codegen.pl -m msg --noTimestamps --noAggregateConversion -p test test_codec.xsd
//..
// The contents of test_codec.xsd follows:
//..
//  <?xml version="1.0" encoding="UTF-8"?>
//  <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema"
//             xmlns:bdem="http://bloomberg.com/schemas/bdem"
//             xmlns="http://bloomberg.com/schemas/test_codec"
//             targetNamespace="http://bloomberg.com/schemas/test_codec"
//             elementFormDefault="qualified">
//      <!-- "$Id: $  $CSID:  $  $SCMId:  $" -->
//
//      <xs:annotation>
//          <xs:documentation>
//              This component provides classes for testing codecs.
//          </xs:documentation>
//      </xs:annotation>
//
//      <xs:simpleType name="CustomizedString">
//          <xs:restriction base="xs:string">
//              <xs:maxLength value="25"/>
//          </xs:restriction>
//      </xs:simpleType>
//
//      <xs:complexType name="MyChoice">
//          <xs:choice>
//              <xs:element name="selection1" type="xs:int"/>
//              <xs:element name="selection2" type="xs:string"/>
//          </xs:choice>
//      </xs:complexType>
//
//      <xs:simpleType name="MyEnumeration" bdem:preserveEnumOrder="1">
//          <xs:restriction base="xs:string">
//              <xs:enumeration value="VALUE1"/>
//              <xs:enumeration value="VALUE2"/>
//          </xs:restriction>
//      </xs:simpleType>
//
//      <xs:complexType name="MySequence">
//          <xs:sequence>
//              <xs:element name="attribute1" type="xs:int"/>
//              <xs:element name="attribute2" type="xs:string"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="MySequenceWithArray">
//          <xs:sequence>
//              <xs:element name="attribute1" type="xs:int"/>
//              <xs:element name="attribute2" type="xs:string"
//                          minOccurs="0" maxOccurs="unbounded"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="MySequenceWithNullable">
//          <xs:sequence>
//              <xs:element name="attribute1" type="xs:int"/>
//              <xs:element name="attribute2" type="xs:string"
//                          minOccurs="0" maxOccurs="1"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="MySequenceWithAnonymousChoice">
//          <xs:sequence>
//              <xs:element name="attribute1" type="xs:int"/>
//              <xs:choice>
//                  <xs:element name="myChoice1" type="xs:int"/>
//                  <xs:element name="myChoice2" type="xs:string"/>
//              </xs:choice>
//              <xs:element name="attribute2" type="xs:string"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="MySequenceWithNillable">
//          <xs:sequence>
//              <xs:element name="attribute1" type="xs:int"/>
//              <xs:element name="myNillable" type="xs:string"
//                          nillable="true"/>
//              <xs:element name="attribute2" type="xs:string"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="Address">
//          <xs:sequence>
//              <xs:element name="street" type="xs:string"/>
//              <xs:element name="city"   type="xs:string"/>
//              <xs:element name="state"  type="xs:string"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="Employee">
//          <xs:sequence>
//              <xs:element name="name"        type="xs:string"/>
//              <xs:element name="homeAddress" type="Address"/>
//              <xs:element name="age"         type="xs:int"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="sqrt">
//          <xs:annotation>
//              <xs:documentation>
//                  Simulate a square root request/response
//              </xs:documentation>
//          </xs:annotation>
//          <xs:sequence>
//              <xs:element name="value" type="xs:double"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="basicRecord">
//          <xs:annotation>
//              <xs:documentation>
//                  A representative small record type
//              </xs:documentation>
//          </xs:annotation>
//          <xs:sequence>
//              <xs:element name="i1" type="xs:int"/>
//              <xs:element name="i2" type="xs:int"/>
//              <xs:element name="dt" type="xs:dateTime"/>
//              <xs:element name="s"  type="xs:string"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="bigRecord">
//          <xs:annotation>
//              <xs:documentation>
//                  A bigger record containing an array of smaller records
//              </xs:documentation>
//          </xs:annotation>
//          <xs:sequence>
//              <xs:element name="name" type="xs:string" />
//              <xs:element name="array" type="basicRecord"
//                          minOccurs="0" maxOccurs="unbounded"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="timingRequest">
//          <xs:annotation>
//              <xs:documentation>
//                  A choice record representative of a typical request object.
//              </xs:documentation>
//          </xs:annotation>
//          <xs:choice>
//              <xs:element name="sqrt" type="sqrt"/>
//              <xs:element name="basic" type="basicRecord"/>
//              <xs:element name="big" type="bigRecord"/>
//          </xs:choice>
//      </xs:complexType>
//
//      <xs:element name="timingRequest" type="timingRequest"/>
//
//      <xs:complexType name="rawData">
//          <xs:sequence>
//              <xs:element name="charvec" type="xs:byte"
//                          minOccurs="0" maxOccurs="unbounded"
//                          bdem:formattingMode="default"
//                          bdem:id="0"/>
//              <xs:element name="ucharvec" type="xs:unsignedByte"
//                          minOccurs="0" maxOccurs="unbounded"
//                          bdem:formattingMode="default"
//                          bdem:id="1"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="rawDataUnformatted">
//          <xs:sequence>
//              <xs:element name="charvec" type="xs:byte"
//                          minOccurs="0" maxOccurs="unbounded"
//                          bdem:id="0"/>
//              <xs:element name="ucharvec" type="xs:unsignedByte"
//                          minOccurs="0" maxOccurs="unbounded"
//                          bdem:id="1"/>
//          </xs:sequence>
//      </xs:complexType>
//
//      <xs:complexType name="rawDataSwitched">
//          <xs:sequence>
//              <xs:element name="charvec" type="xs:byte"
//                          minOccurs="0" maxOccurs="unbounded"
//                          bdem:id="1"/>
//              <xs:element name="ucharvec" type="xs:unsignedByte"
//                          minOccurs="0" maxOccurs="unbounded"
//                          bdem:id="0"/>
//          </xs:sequence>
//      </xs:complexType>
//  </xs:schema>

// ************************ START OF GENERATED CODE **************************
// test_schema.h               *DO NOT EDIT*               @generated -*-C++-*-
#ifndef INCLUDED_TEST_SCHEMA
#define INCLUDED_TEST_SCHEMA

//@PURPOSE: Provide value-semantic attribute classes

#include <bslalg_typetraits.h>

#include <bdlat_attributeinfo.h>

#include <bdlat_enumeratorinfo.h>

#include <bdlat_selectioninfo.h>

#include <bdlat_typetraits.h>

#include <bdlat_valuetypefunctions.h>

#include <bsls_objectbuffer.h>

#include <bslx_instreamfunctions.h>
#include <bslx_outstreamfunctions.h>

#include <bslma_default.h>

#include <bsls_assert.h>

#include <bdlb_nullablevalue.h>

#include <bdlt_datetimetz.h>

#include <bsl_string.h>

#include <bsl_vector.h>

#include <bsl_iosfwd.h>

#include <bsl_ostream.h>
#include <bsl_string.h>

namespace BloombergLP {

namespace bslma { class Allocator; }

namespace test { class Address; }
namespace test { class BasicRecord; }
namespace test { class CustomizedString; }
namespace test { class MyChoice; }
namespace test { class MySequence; }
namespace test { class MySequenceWithAnonymousChoiceChoice; }
namespace test { class MySequenceWithArray; }
namespace test { class MySequenceWithNillable; }
namespace test { class MySequenceWithNullable; }
namespace test { class RawData; }
namespace test { class RawDataSwitched; }
namespace test { class RawDataUnformatted; }
namespace test { class Sqrt; }
namespace test { class BigRecord; }
namespace test { class Employee; }
namespace test { class MyChoiceWithDefaultValues; }
namespace test { class MySequenceWithAnonymousChoice; }
namespace test { class MySequenceWithDefaultValues; }
namespace test { class TimingRequest; }
namespace test {

                               // =============
                               // class Address
                               // =============

class Address {

    // INSTANCE DATA
    bsl::string  d_street;
    bsl::string  d_city;
    bsl::string  d_state;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_STREET = 0
      , ATTRIBUTE_ID_CITY   = 1
      , ATTRIBUTE_ID_STATE  = 2
    };

    enum {
        NUM_ATTRIBUTES = 3
    };

    enum {
        ATTRIBUTE_INDEX_STREET = 0
      , ATTRIBUTE_INDEX_CITY   = 1
      , ATTRIBUTE_INDEX_STATE  = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit Address(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'Address' having the default value.  Use
        // the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    Address(const Address& original,
            bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'Address' having the value of the specified
        // 'original' object.  Use the optionally specified 'basicAllocator' to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    ~Address();
        // Destroy this object.

    // MANIPULATORS
    Address& operator=(const Address& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    bsl::string& street();
        // Return a reference to the modifiable "Street" attribute of this
        // object.

    bsl::string& city();
        // Return a reference to the modifiable "City" attribute of this
        // object.

    bsl::string& state();
        // Return a reference to the modifiable "State" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    const bsl::string& street() const;
        // Return a reference to the non-modifiable "Street" attribute of this
        // object.

    const bsl::string& city() const;
        // Return a reference to the non-modifiable "City" attribute of this
        // object.

    const bsl::string& state() const;
        // Return a reference to the non-modifiable "State" attribute of this
        // object.
};

// FREE OPERATORS
inline
bool operator==(const Address& lhs, const Address& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const Address& lhs, const Address& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const Address& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::Address)

namespace test {

                             // =================
                             // class BasicRecord
                             // =================

class BasicRecord {
    // A representative small record type

    // INSTANCE DATA
    bsl::string       d_s;
    bdlt::DatetimeTz  d_dt;
    int               d_i1;
    int               d_i2;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_I1 = 0
      , ATTRIBUTE_ID_I2 = 1
      , ATTRIBUTE_ID_DT = 2
      , ATTRIBUTE_ID_S  = 3
    };

    enum {
        NUM_ATTRIBUTES = 4
    };

    enum {
        ATTRIBUTE_INDEX_I1 = 0
      , ATTRIBUTE_INDEX_I2 = 1
      , ATTRIBUTE_INDEX_DT = 2
      , ATTRIBUTE_INDEX_S  = 3
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit BasicRecord(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'BasicRecord' having the default value.
        // Use the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    BasicRecord(const BasicRecord& original,
                bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'BasicRecord' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~BasicRecord();
        // Destroy this object.

    // MANIPULATORS
    BasicRecord& operator=(const BasicRecord& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& i1();
        // Return a reference to the modifiable "I1" attribute of this object.

    int& i2();
        // Return a reference to the modifiable "I2" attribute of this object.

    bdlt::DatetimeTz& dt();
        // Return a reference to the modifiable "Dt" attribute of this object.

    bsl::string& s();
        // Return a reference to the modifiable "S" attribute of this object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int i1() const;
        // Return a reference to the non-modifiable "I1" attribute of this
        // object.

    int i2() const;
        // Return a reference to the non-modifiable "I2" attribute of this
        // object.

    const bdlt::DatetimeTz& dt() const;
        // Return a reference to the non-modifiable "Dt" attribute of this
        // object.

    const bsl::string& s() const;
        // Return a reference to the non-modifiable "S" attribute of this
        // object.
};

// FREE OPERATORS
inline
bool operator==(const BasicRecord& lhs, const BasicRecord& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const BasicRecord& lhs, const BasicRecord& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const BasicRecord& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::BasicRecord)

namespace test {

                           // ======================
                           // class CustomizedString
                           // ======================

class CustomizedString {

    // INSTANCE DATA
    bsl::string d_value;

    // FRIENDS
    friend bool operator==(const CustomizedString& lhs, const CustomizedString& rhs);
    friend bool operator!=(const CustomizedString& lhs, const CustomizedString& rhs);

  public:
    // TYPES
    typedef bsl::string BaseType;

    // CONSTANTS
    static const char CLASS_NAME[];

    // CREATORS
    explicit CustomizedString(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'CustomizedString' having the default
        // value.  Use the optionally specified 'basicAllocator' to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    CustomizedString(const CustomizedString& original,
                    bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'CustomizedString' having the value
        // of the specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0,
        // the currently installed default allocator is used.

    explicit CustomizedString(const bsl::string& value,
                             bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'CustomizedString' having the specified
        // 'value'.  Use the optionally specified 'basicAllocator' to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    ~CustomizedString();
        // Destroy this object.

    // MANIPULATORS
    CustomizedString& operator=(const CustomizedString& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    int fromString(const bsl::string& value);
        // Convert from the specified 'value' to this type.  Return 0 if
        // successful and non-zero otherwise.

    // ACCESSORS
    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    int maxSupportedBdexVersion() const;
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    const bsl::string& toString() const;
        // Convert this value to 'bsl::string'.

    // PUBLIC CLASS METHODS
    static int checkRestrictions(const bsl::string& value);
        // Check if the specified 'value' satisfies the restrictions of this
        // class (i.e., "CustomizedString").  Return 0 if successful (i.e., the
        // restrictions are satisfied) and non-zero otherwise.
};

// FREE OPERATORS
inline
bool operator==(const CustomizedString& lhs, const CustomizedString& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const CustomizedString& lhs, const CustomizedString& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const CustomizedString& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_CUSTOMIZEDTYPE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::CustomizedString)

namespace test {

                               // ==============
                               // class MyChoice
                               // ==============

class MyChoice {

    // INSTANCE DATA
    union {
        bsls::ObjectBuffer< int >         d_selection1;
        bsls::ObjectBuffer< bsl::string > d_selection2;
    };

    int                                   d_selectionId;
    bslma::Allocator                     *d_allocator_p;

  public:
    // TYPES

    enum {
        SELECTION_ID_UNDEFINED  = -1
      , SELECTION_ID_SELECTION1 = 0
      , SELECTION_ID_SELECTION2 = 1
    };

    enum {
        NUM_SELECTIONS = 2
    };

    enum {
        SELECTION_INDEX_SELECTION1 = 0
      , SELECTION_INDEX_SELECTION2 = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_SelectionInfo SELECTION_INFO_ARRAY[];

    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_SelectionInfo *lookupSelectionInfo(int id);
        // Return selection information for the selection indicated by the
        // specified 'id' if the selection exists, and 0 otherwise.

    static const bdlat_SelectionInfo *lookupSelectionInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return selection information for the selection indicated by the
        // specified 'name' of the specified 'nameLength' if the selection
        // exists, and 0 otherwise.

    // CREATORS
    explicit MyChoice(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MyChoice' having the default value.  Use
        // the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    MyChoice(const MyChoice& original,
            bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MyChoice' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~MyChoice();
        // Destroy this object.

    // MANIPULATORS
    MyChoice& operator=(const MyChoice& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon default
        // construction).

    int makeSelection(int selectionId);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'selectionId'.  Return 0 on success, and
        // non-zero value otherwise (i.e., the selection is not found).

    int makeSelection(const char *name, int nameLength);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'name' of the specified 'nameLength'.
        // Return 0 on success, and non-zero value otherwise (i.e., the
        // selection is not found).

    int& makeSelection1();
    int& makeSelection1(int value);
        // Set the value of this object to be a "Selection1" value.  Optionally
        // specify the 'value' of the "Selection1".  If 'value' is not
        // specified, the default "Selection1" value is used.

    bsl::string& makeSelection2();
    bsl::string& makeSelection2(const bsl::string& value);
        // Set the value of this object to be a "Selection2" value.  Optionally
        // specify the 'value' of the "Selection2".  If 'value' is not
        // specified, the default "Selection2" value is used.

    template<class MANIPULATOR>
    int manipulateSelection(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' on the address of the modifiable
        // selection, supplying 'manipulator' with the corresponding selection
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if this object has a defined selection,
        // and -1 otherwise.

    int& selection1();
        // Return a reference to the modifiable "Selection1" selection of this
        // object if "Selection1" is the current selection.  The behavior is
        // undefined unless "Selection1" is the selection of this object.

    bsl::string& selection2();
        // Return a reference to the modifiable "Selection2" selection of this
        // object if "Selection2" is the current selection.  The behavior is
        // undefined unless "Selection2" is the selection of this object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    int selectionId() const;
        // Return the id of the current selection if the selection is defined,
        // and -1 otherwise.

    template<class ACCESSOR>
    int accessSelection(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' on the non-modifiable selection,
        // supplying 'accessor' with the corresponding selection information
        // structure.  Return the value returned from the invocation of
        // 'accessor' if this object has a defined selection, and -1 otherwise.

    const int& selection1() const;
        // Return a reference to the non-modifiable "Selection1" selection of
        // this object if "Selection1" is the current selection.  The behavior
        // is undefined unless "Selection1" is the selection of this object.

    const bsl::string& selection2() const;
        // Return a reference to the non-modifiable "Selection2" selection of
        // this object if "Selection2" is the current selection.  The behavior
        // is undefined unless "Selection2" is the selection of this object.

    bool isSelection1Value() const;
        // Return 'true' if the value of this object is a "Selection1" value,
        // and return 'false' otherwise.

    bool isSelection2Value() const;
        // Return 'true' if the value of this object is a "Selection2" value,
        // and return 'false' otherwise.

    bool isUndefinedValue() const;
        // Return 'true' if the value of this object is undefined, and 'false'
        // otherwise.

    const char *selectionName() const;
        // Return the symbolic name of the current selection of this object.
};

// FREE OPERATORS
inline
bool operator==(const MyChoice& lhs, const MyChoice& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
    // value, and 'false' otherwise.  Two 'MyChoice' objects have the same
    // value if either the selections in both objects have the same ids and
    // the same values, or both selections are undefined.

inline
bool operator!=(const MyChoice& lhs, const MyChoice& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects do not have the
    // same values, as determined by 'operator==', and 'false' otherwise.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MyChoice& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_CHOICE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MyChoice)

namespace test {

                            // ===================
                            // class MyEnumeration
                            // ===================

struct MyEnumeration {

  public:
    // TYPES
    enum Value {
        VALUE1 = 0
      , VALUE2 = 1
    };

    enum {
        NUM_ENUMERATORS = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_EnumeratorInfo ENUMERATOR_INFO_ARRAY[];

    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const char *toString(Value value);
        // Return the string representation exactly matching the enumerator
        // name corresponding to the specified enumeration 'value'.

    static int fromString(Value        *result,
                          const char   *string,
                          int           stringLength);
        // Load into the specified 'result' the enumerator matching the
        // specified 'string' of the specified 'stringLength'.  Return 0 on
        // success, and a non-zero value with no effect on 'result' otherwise
        // (i.e., 'string' does not match any enumerator).

    static int fromString(Value              *result,
                          const bsl::string&  string);
        // Load into the specified 'result' the enumerator matching the
        // specified 'string'.  Return 0 on success, and a non-zero value with
        // no effect on 'result' otherwise (i.e., 'string' does not match any
        // enumerator).

    static int fromInt(Value *result, int number);
        // Load into the specified 'result' the enumerator matching the
        // specified 'number'.  Return 0 on success, and a non-zero value with
        // no effect on 'result' otherwise (i.e., 'number' does not match any
        // enumerator).

    template <class STREAM>
    static STREAM& bdexStreamIn(STREAM&  stream,
                                Value&   value,
                                int      version);
        // Assign to the specified 'value' the value read from the specified
        // input 'stream' using the specified 'version' format and return a
        // reference to the modifiable 'stream'.  If 'stream' is initially
        // invalid, this operation has no effect.  If 'stream' becomes invalid
        // during this operation, the 'value' is valid, but its value is
        // undefined.  If the specified 'version' is not supported, 'stream' is
        // marked invalid, but 'value' is unaltered.  Note that no version is
        // read from 'stream'.  (See the package-group-level documentation for
        // more information on 'bdex' streaming of container types.)

    static bsl::ostream& print(bsl::ostream& stream, Value value);
        // Write to the specified 'stream' the string representation of
        // the specified enumeration 'value'.  Return a reference to
        // the modifiable 'stream'.

    template <class STREAM>
    static STREAM& bdexStreamOut(STREAM&  stream,
                                 Value    value,
                                 int      version);
        // Write the specified 'value' to the specified output 'stream' and
        // return a reference to the modifiable 'stream'.  Optionally specify
        // an explicit 'version' format; by default, the maximum supported
        // version is written to 'stream' and used as the format.  If 'version'
        // is specified, that format is used but *not* written to 'stream'.  If
        // 'version' is not supported, 'stream' is left unmodified.  (See the
        // package-group-level documentation for more information on 'bdex'
        // streaming of container types).
};

// FREE OPERATORS
inline
bsl::ostream& operator<<(bsl::ostream& stream, MyEnumeration::Value rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_ENUMERATION_TRAITS(test::MyEnumeration)


namespace test {

                              // ================
                              // class MySequence
                              // ================

class MySequence {

    // INSTANCE DATA
    bsl::string  d_attribute2;
    int          d_attribute1;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_ATTRIBUTE1 = 0
      , ATTRIBUTE_ID_ATTRIBUTE2 = 1
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_ATTRIBUTE1 = 0
      , ATTRIBUTE_INDEX_ATTRIBUTE2 = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequence(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequence' having the default value.  Use
        // the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    MySequence(const MySequence& original,
               bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequence' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~MySequence();
        // Destroy this object.

    // MANIPULATORS
    MySequence& operator=(const MySequence& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& attribute1();
        // Return a reference to the modifiable "Attribute1" attribute of this
        // object.

    bsl::string& attribute2();
        // Return a reference to the modifiable "Attribute2" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int attribute1() const;
        // Return a reference to the non-modifiable "Attribute1" attribute of
        // this object.

    const bsl::string& attribute2() const;
        // Return a reference to the non-modifiable "Attribute2" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequence& lhs, const MySequence& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const MySequence& lhs, const MySequence& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequence& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequence)

namespace test {

                 // =========================================
                 // class MySequenceWithAnonymousChoiceChoice
                 // =========================================

class MySequenceWithAnonymousChoiceChoice {

    // INSTANCE DATA
    union {
        bsls::ObjectBuffer< int >         d_myChoice1;
        bsls::ObjectBuffer< bsl::string > d_myChoice2;
    };

    int                                   d_selectionId;
    bslma::Allocator                     *d_allocator_p;

  public:
    // TYPES

    enum {
        SELECTION_ID_UNDEFINED  = -1
      , SELECTION_ID_MY_CHOICE1 = 0
      , SELECTION_ID_MY_CHOICE2 = 1
    };

    enum {
        NUM_SELECTIONS = 2
    };

    enum {
        SELECTION_INDEX_MY_CHOICE1 = 0
      , SELECTION_INDEX_MY_CHOICE2 = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_SelectionInfo SELECTION_INFO_ARRAY[];

    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_SelectionInfo *lookupSelectionInfo(int id);
        // Return selection information for the selection indicated by the
        // specified 'id' if the selection exists, and 0 otherwise.

    static const bdlat_SelectionInfo *lookupSelectionInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return selection information for the selection indicated by the
        // specified 'name' of the specified 'nameLength' if the selection
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequenceWithAnonymousChoiceChoice(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithAnonymousChoiceChoice'
        // having the default value.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    MySequenceWithAnonymousChoiceChoice(const MySequenceWithAnonymousChoiceChoice& original,
                                       bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithAnonymousChoiceChoice'
        // having the value of the specified 'original' object.  Use the
        // optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    ~MySequenceWithAnonymousChoiceChoice();
        // Destroy this object.

    // MANIPULATORS
    MySequenceWithAnonymousChoiceChoice& operator=(const MySequenceWithAnonymousChoiceChoice& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon default
        // construction).

    int makeSelection(int selectionId);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'selectionId'.  Return 0 on success, and
        // non-zero value otherwise (i.e., the selection is not found).

    int makeSelection(const char *name, int nameLength);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'name' of the specified 'nameLength'.
        // Return 0 on success, and non-zero value otherwise (i.e., the
        // selection is not found).

    int& makeMyChoice1();
    int& makeMyChoice1(int value);
        // Set the value of this object to be a "MyChoice1" value.  Optionally
        // specify the 'value' of the "MyChoice1".  If 'value' is not
        // specified, the default "MyChoice1" value is used.

    bsl::string& makeMyChoice2();
    bsl::string& makeMyChoice2(const bsl::string& value);
        // Set the value of this object to be a "MyChoice2" value.  Optionally
        // specify the 'value' of the "MyChoice2".  If 'value' is not
        // specified, the default "MyChoice2" value is used.

    template<class MANIPULATOR>
    int manipulateSelection(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' on the address of the modifiable
        // selection, supplying 'manipulator' with the corresponding selection
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if this object has a defined selection,
        // and -1 otherwise.

    int& myChoice1();
        // Return a reference to the modifiable "MyChoice1" selection of this
        // object if "MyChoice1" is the current selection.  The behavior is
        // undefined unless "MyChoice1" is the selection of this object.

    bsl::string& myChoice2();
        // Return a reference to the modifiable "MyChoice2" selection of this
        // object if "MyChoice2" is the current selection.  The behavior is
        // undefined unless "MyChoice2" is the selection of this object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    int selectionId() const;
        // Return the id of the current selection if the selection is defined,
        // and -1 otherwise.

    template<class ACCESSOR>
    int accessSelection(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' on the non-modifiable selection,
        // supplying 'accessor' with the corresponding selection information
        // structure.  Return the value returned from the invocation of
        // 'accessor' if this object has a defined selection, and -1 otherwise.

    const int& myChoice1() const;
        // Return a reference to the non-modifiable "MyChoice1" selection of
        // this object if "MyChoice1" is the current selection.  The behavior
        // is undefined unless "MyChoice1" is the selection of this object.

    const bsl::string& myChoice2() const;
        // Return a reference to the non-modifiable "MyChoice2" selection of
        // this object if "MyChoice2" is the current selection.  The behavior
        // is undefined unless "MyChoice2" is the selection of this object.

    bool isMyChoice1Value() const;
        // Return 'true' if the value of this object is a "MyChoice1" value,
        // and return 'false' otherwise.

    bool isMyChoice2Value() const;
        // Return 'true' if the value of this object is a "MyChoice2" value,
        // and return 'false' otherwise.

    bool isUndefinedValue() const;
        // Return 'true' if the value of this object is undefined, and 'false'
        // otherwise.

    const char *selectionName() const;
        // Return the symbolic name of the current selection of this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequenceWithAnonymousChoiceChoice& lhs, const MySequenceWithAnonymousChoiceChoice& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
    // value, and 'false' otherwise.  Two 'MySequenceWithAnonymousChoiceChoice' objects have the same
    // value if either the selections in both objects have the same ids and
    // the same values, or both selections are undefined.

inline
bool operator!=(const MySequenceWithAnonymousChoiceChoice& lhs, const MySequenceWithAnonymousChoiceChoice& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects do not have the
    // same values, as determined by 'operator==', and 'false' otherwise.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequenceWithAnonymousChoiceChoice& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_CHOICE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequenceWithAnonymousChoiceChoice)

namespace test {

                         // =========================
                         // class MySequenceWithArray
                         // =========================

class MySequenceWithArray {

    // INSTANCE DATA
    bsl::vector<bsl::string>  d_attribute2;
    int                       d_attribute1;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_ATTRIBUTE1 = 0
      , ATTRIBUTE_ID_ATTRIBUTE2 = 1
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_ATTRIBUTE1 = 0
      , ATTRIBUTE_INDEX_ATTRIBUTE2 = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequenceWithArray(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithArray' having the default
        // value.  Use the optionally specified 'basicAllocator' to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    MySequenceWithArray(const MySequenceWithArray& original,
                        bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithArray' having the value of
        // the specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~MySequenceWithArray();
        // Destroy this object.

    // MANIPULATORS
    MySequenceWithArray& operator=(const MySequenceWithArray& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& attribute1();
        // Return a reference to the modifiable "Attribute1" attribute of this
        // object.

    bsl::vector<bsl::string>& attribute2();
        // Return a reference to the modifiable "Attribute2" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int attribute1() const;
        // Return a reference to the non-modifiable "Attribute1" attribute of
        // this object.

    const bsl::vector<bsl::string>& attribute2() const;
        // Return a reference to the non-modifiable "Attribute2" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequenceWithArray& lhs, const MySequenceWithArray& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const MySequenceWithArray& lhs, const MySequenceWithArray& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequenceWithArray& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequenceWithArray)

namespace test {

                        // ============================
                        // class MySequenceWithNillable
                        // ============================

class MySequenceWithNillable {

    // INSTANCE DATA
    bsl::string                       d_attribute2;
    bdlb::NullableValue<bsl::string>  d_myNillable;
    int                               d_attribute1;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_ATTRIBUTE1  = 0
      , ATTRIBUTE_ID_MY_NILLABLE = 1
      , ATTRIBUTE_ID_ATTRIBUTE2  = 2
    };

    enum {
        NUM_ATTRIBUTES = 3
    };

    enum {
        ATTRIBUTE_INDEX_ATTRIBUTE1  = 0
      , ATTRIBUTE_INDEX_MY_NILLABLE = 1
      , ATTRIBUTE_INDEX_ATTRIBUTE2  = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequenceWithNillable(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithNillable' having the default
        // value.  Use the optionally specified 'basicAllocator' to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    MySequenceWithNillable(const MySequenceWithNillable& original,
                           bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithNillable' having the value
        // of the specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~MySequenceWithNillable();
        // Destroy this object.

    // MANIPULATORS
    MySequenceWithNillable& operator=(const MySequenceWithNillable& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& attribute1();
        // Return a reference to the modifiable "Attribute1" attribute of this
        // object.

    bdlb::NullableValue<bsl::string>& myNillable();
        // Return a reference to the modifiable "MyNillable" attribute of this
        // object.

    bsl::string& attribute2();
        // Return a reference to the modifiable "Attribute2" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int attribute1() const;
        // Return a reference to the non-modifiable "Attribute1" attribute of
        // this object.

    const bdlb::NullableValue<bsl::string>& myNillable() const;
        // Return a reference to the non-modifiable "MyNillable" attribute of
        // this object.

    const bsl::string& attribute2() const;
        // Return a reference to the non-modifiable "Attribute2" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequenceWithNillable& lhs, const MySequenceWithNillable& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const MySequenceWithNillable& lhs, const MySequenceWithNillable& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequenceWithNillable& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequenceWithNillable)

namespace test {

                        // ============================
                        // class MySequenceWithNullable
                        // ============================

class MySequenceWithNullable {

    // INSTANCE DATA
    bdlb::NullableValue<bsl::string>  d_attribute2;
    int                               d_attribute1;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_ATTRIBUTE1 = 0
      , ATTRIBUTE_ID_ATTRIBUTE2 = 1
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_ATTRIBUTE1 = 0
      , ATTRIBUTE_INDEX_ATTRIBUTE2 = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequenceWithNullable(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithNullable' having the default
        // value.  Use the optionally specified 'basicAllocator' to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    MySequenceWithNullable(const MySequenceWithNullable& original,
                           bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithNullable' having the value
        // of the specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~MySequenceWithNullable();
        // Destroy this object.

    // MANIPULATORS
    MySequenceWithNullable& operator=(const MySequenceWithNullable& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& attribute1();
        // Return a reference to the modifiable "Attribute1" attribute of this
        // object.

    bdlb::NullableValue<bsl::string>& attribute2();
        // Return a reference to the modifiable "Attribute2" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int attribute1() const;
        // Return a reference to the non-modifiable "Attribute1" attribute of
        // this object.

    const bdlb::NullableValue<bsl::string>& attribute2() const;
        // Return a reference to the non-modifiable "Attribute2" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequenceWithNullable& lhs, const MySequenceWithNullable& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const MySequenceWithNullable& lhs, const MySequenceWithNullable& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequenceWithNullable& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequenceWithNullable)

namespace test {

                               // =============
                               // class RawData
                               // =============

class RawData {

    // INSTANCE DATA
    bsl::vector<unsigned char>  d_ucharvec;
    bsl::vector<char>           d_charvec;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_CHARVEC  = 0
      , ATTRIBUTE_ID_UCHARVEC = 1
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_CHARVEC  = 0
      , ATTRIBUTE_INDEX_UCHARVEC = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit RawData(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'RawData' having the default value.  Use
        // the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    RawData(const RawData& original,
            bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'RawData' having the value of the specified
        // 'original' object.  Use the optionally specified 'basicAllocator' to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    ~RawData();
        // Destroy this object.

    // MANIPULATORS
    RawData& operator=(const RawData& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    bsl::vector<char>& charvec();
        // Return a reference to the modifiable "Charvec" attribute of this
        // object.

    bsl::vector<unsigned char>& ucharvec();
        // Return a reference to the modifiable "Ucharvec" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    const bsl::vector<char>& charvec() const;
        // Return a reference to the non-modifiable "Charvec" attribute of this
        // object.

    const bsl::vector<unsigned char>& ucharvec() const;
        // Return a reference to the non-modifiable "Ucharvec" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const RawData& lhs, const RawData& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const RawData& lhs, const RawData& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const RawData& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::RawData)

namespace test {

                           // =====================
                           // class RawDataSwitched
                           // =====================

class RawDataSwitched {

    // INSTANCE DATA
    bsl::vector<unsigned char>  d_ucharvec;
    bsl::vector<char>           d_charvec;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_CHARVEC  = 1
      , ATTRIBUTE_ID_UCHARVEC = 0
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_CHARVEC  = 0
      , ATTRIBUTE_INDEX_UCHARVEC = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit RawDataSwitched(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'RawDataSwitched' having the default value.
        //  Use the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    RawDataSwitched(const RawDataSwitched& original,
                    bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'RawDataSwitched' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~RawDataSwitched();
        // Destroy this object.

    // MANIPULATORS
    RawDataSwitched& operator=(const RawDataSwitched& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    bsl::vector<char>& charvec();
        // Return a reference to the modifiable "Charvec" attribute of this
        // object.

    bsl::vector<unsigned char>& ucharvec();
        // Return a reference to the modifiable "Ucharvec" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    const bsl::vector<char>& charvec() const;
        // Return a reference to the non-modifiable "Charvec" attribute of this
        // object.

    const bsl::vector<unsigned char>& ucharvec() const;
        // Return a reference to the non-modifiable "Ucharvec" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const RawDataSwitched& lhs, const RawDataSwitched& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const RawDataSwitched& lhs, const RawDataSwitched& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const RawDataSwitched& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::RawDataSwitched)

namespace test {

                          // ========================
                          // class RawDataUnformatted
                          // ========================

class RawDataUnformatted {

    // INSTANCE DATA
    bsl::vector<unsigned char>  d_ucharvec;
    bsl::vector<char>           d_charvec;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_CHARVEC  = 0
      , ATTRIBUTE_ID_UCHARVEC = 1
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_CHARVEC  = 0
      , ATTRIBUTE_INDEX_UCHARVEC = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit RawDataUnformatted(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'RawDataUnformatted' having the default
        // value.  Use the optionally specified 'basicAllocator' to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    RawDataUnformatted(const RawDataUnformatted& original,
                       bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'RawDataUnformatted' having the value of
        // the specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~RawDataUnformatted();
        // Destroy this object.

    // MANIPULATORS
    RawDataUnformatted& operator=(const RawDataUnformatted& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    bsl::vector<char>& charvec();
        // Return a reference to the modifiable "Charvec" attribute of this
        // object.

    bsl::vector<unsigned char>& ucharvec();
        // Return a reference to the modifiable "Ucharvec" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    const bsl::vector<char>& charvec() const;
        // Return a reference to the non-modifiable "Charvec" attribute of this
        // object.

    const bsl::vector<unsigned char>& ucharvec() const;
        // Return a reference to the non-modifiable "Ucharvec" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const RawDataUnformatted& lhs, const RawDataUnformatted& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const RawDataUnformatted& lhs, const RawDataUnformatted& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const RawDataUnformatted& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::RawDataUnformatted)

namespace test {

                                 // ==========
                                 // class Sqrt
                                 // ==========

class Sqrt {
    // Simulate a square root request/response

    // INSTANCE DATA
    double  d_value;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_VALUE = 0
    };

    enum {
        NUM_ATTRIBUTES = 1
    };

    enum {
        ATTRIBUTE_INDEX_VALUE = 0
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    Sqrt();
        // Create an object of type 'Sqrt' having the default value.

    Sqrt(const Sqrt& original);
        // Create an object of type 'Sqrt' having the value of the specified
        // 'original' object.

    ~Sqrt();
        // Destroy this object.

    // MANIPULATORS
    Sqrt& operator=(const Sqrt& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    double& value();
        // Return a reference to the modifiable "Value" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    double value() const;
        // Return a reference to the non-modifiable "Value" attribute of this
        // object.
};

// FREE OPERATORS
inline
bool operator==(const Sqrt& lhs, const Sqrt& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const Sqrt& lhs, const Sqrt& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const Sqrt& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_BITWISEMOVEABLE_TRAITS(test::Sqrt)

namespace test {

                              // ===============
                              // class BigRecord
                              // ===============

class BigRecord {
    // A bigger record containing an array of smaller records

    // INSTANCE DATA
    bsl::vector<BasicRecord>  d_array;
    bsl::string               d_name;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_NAME  = 0
      , ATTRIBUTE_ID_ARRAY = 1
    };

    enum {
        NUM_ATTRIBUTES = 2
    };

    enum {
        ATTRIBUTE_INDEX_NAME  = 0
      , ATTRIBUTE_INDEX_ARRAY = 1
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit BigRecord(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'BigRecord' having the default value.  Use
        // the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    BigRecord(const BigRecord& original,
              bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'BigRecord' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~BigRecord();
        // Destroy this object.

    // MANIPULATORS
    BigRecord& operator=(const BigRecord& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    bsl::string& name();
        // Return a reference to the modifiable "Name" attribute of this
        // object.

    bsl::vector<BasicRecord>& array();
        // Return a reference to the modifiable "Array" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    const bsl::string& name() const;
        // Return a reference to the non-modifiable "Name" attribute of this
        // object.

    const bsl::vector<BasicRecord>& array() const;
        // Return a reference to the non-modifiable "Array" attribute of this
        // object.
};

// FREE OPERATORS
inline
bool operator==(const BigRecord& lhs, const BigRecord& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const BigRecord& lhs, const BigRecord& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const BigRecord& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::BigRecord)

namespace test {

                               // ==============
                               // class Employee
                               // ==============

class Employee {

    // INSTANCE DATA
    bsl::string  d_name;
    Address      d_homeAddress;
    int          d_age;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_NAME         = 0
      , ATTRIBUTE_ID_HOME_ADDRESS = 1
      , ATTRIBUTE_ID_AGE          = 2
    };

    enum {
        NUM_ATTRIBUTES = 3
    };

    enum {
        ATTRIBUTE_INDEX_NAME         = 0
      , ATTRIBUTE_INDEX_HOME_ADDRESS = 1
      , ATTRIBUTE_INDEX_AGE          = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit Employee(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'Employee' having the default value.  Use
        // the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    Employee(const Employee& original,
             bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'Employee' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~Employee();
        // Destroy this object.

    // MANIPULATORS
    Employee& operator=(const Employee& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    bsl::string& name();
        // Return a reference to the modifiable "Name" attribute of this
        // object.

    Address& homeAddress();
        // Return a reference to the modifiable "HomeAddress" attribute of this
        // object.

    int& age();
        // Return a reference to the modifiable "Age" attribute of this object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    const bsl::string& name() const;
        // Return a reference to the non-modifiable "Name" attribute of this
        // object.

    const Address& homeAddress() const;
        // Return a reference to the non-modifiable "HomeAddress" attribute of
        // this object.

    int age() const;
        // Return a reference to the non-modifiable "Age" attribute of this
        // object.
};

// FREE OPERATORS
inline
bool operator==(const Employee& lhs, const Employee& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const Employee& lhs, const Employee& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const Employee& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::Employee)

namespace test {

                      // ===============================
                      // class MyChoiceWithDefaultValues
                      // ===============================

class MyChoiceWithDefaultValues {

    // INSTANCE DATA
    union {
        bsls::ObjectBuffer< int >                  d_selection0;
        bsls::ObjectBuffer< bsl::string >          d_selection1;
        bsls::ObjectBuffer< MyEnumeration::Value > d_selection2;
    };

    int                                            d_selectionId;
    bslma::Allocator                              *d_allocator_p;

  public:
    // TYPES

    enum {
        SELECTION_ID_UNDEFINED  = -1
      , SELECTION_ID_SELECTION0 = 0
      , SELECTION_ID_SELECTION1 = 1
      , SELECTION_ID_SELECTION2 = 2
    };

    enum {
        NUM_SELECTIONS = 3
    };

    enum {
        SELECTION_INDEX_SELECTION0 = 0
      , SELECTION_INDEX_SELECTION1 = 1
      , SELECTION_INDEX_SELECTION2 = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const int DEFAULT_INITIALIZER_SELECTION0;

    static const char DEFAULT_INITIALIZER_SELECTION1[];

    static const MyEnumeration::Value DEFAULT_INITIALIZER_SELECTION2;

    static const bdlat_SelectionInfo SELECTION_INFO_ARRAY[];

    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_SelectionInfo *lookupSelectionInfo(int id);
        // Return selection information for the selection indicated by the
        // specified 'id' if the selection exists, and 0 otherwise.

    static const bdlat_SelectionInfo *lookupSelectionInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return selection information for the selection indicated by the
        // specified 'name' of the specified 'nameLength' if the selection
        // exists, and 0 otherwise.

    // CREATORS
    explicit MyChoiceWithDefaultValues(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MyChoiceWithDefaultValues' having the
        // default value.  Use the optionally specified 'basicAllocator' to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    MyChoiceWithDefaultValues(const MyChoiceWithDefaultValues& original,
                             bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MyChoiceWithDefaultValues' having the
        // value of the specified 'original' object.  Use the optionally
        // specified 'basicAllocator' to supply memory.  If 'basicAllocator' is
        // 0, the currently installed default allocator is used.

    ~MyChoiceWithDefaultValues();
        // Destroy this object.

    // MANIPULATORS
    MyChoiceWithDefaultValues& operator=(const MyChoiceWithDefaultValues& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon default
        // construction).

    int makeSelection(int selectionId);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'selectionId'.  Return 0 on success, and
        // non-zero value otherwise (i.e., the selection is not found).

    int makeSelection(const char *name, int nameLength);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'name' of the specified 'nameLength'.
        // Return 0 on success, and non-zero value otherwise (i.e., the
        // selection is not found).

    int& makeSelection0();
    int& makeSelection0(int value);
        // Set the value of this object to be a "Selection0" value.  Optionally
        // specify the 'value' of the "Selection0".  If 'value' is not
        // specified, the default "Selection0" value is used.

    bsl::string& makeSelection1();
    bsl::string& makeSelection1(const bsl::string& value);
        // Set the value of this object to be a "Selection1" value.  Optionally
        // specify the 'value' of the "Selection1".  If 'value' is not
        // specified, the default "Selection1" value is used.

    MyEnumeration::Value& makeSelection2();
    MyEnumeration::Value& makeSelection2(MyEnumeration::Value value);
        // Set the value of this object to be a "Selection2" value.  Optionally
        // specify the 'value' of the "Selection2".  If 'value' is not
        // specified, the default "Selection2" value is used.

    template<class MANIPULATOR>
    int manipulateSelection(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' on the address of the modifiable
        // selection, supplying 'manipulator' with the corresponding selection
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if this object has a defined selection,
        // and -1 otherwise.

    int& selection0();
        // Return a reference to the modifiable "Selection0" selection of this
        // object if "Selection0" is the current selection.  The behavior is
        // undefined unless "Selection0" is the selection of this object.

    bsl::string& selection1();
        // Return a reference to the modifiable "Selection1" selection of this
        // object if "Selection1" is the current selection.  The behavior is
        // undefined unless "Selection1" is the selection of this object.

    MyEnumeration::Value& selection2();
        // Return a reference to the modifiable "Selection2" selection of this
        // object if "Selection2" is the current selection.  The behavior is
        // undefined unless "Selection2" is the selection of this object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    int selectionId() const;
        // Return the id of the current selection if the selection is defined,
        // and -1 otherwise.

    template<class ACCESSOR>
    int accessSelection(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' on the non-modifiable selection,
        // supplying 'accessor' with the corresponding selection information
        // structure.  Return the value returned from the invocation of
        // 'accessor' if this object has a defined selection, and -1 otherwise.

    const int& selection0() const;
        // Return a reference to the non-modifiable "Selection0" selection of
        // this object if "Selection0" is the current selection.  The behavior
        // is undefined unless "Selection0" is the selection of this object.

    const bsl::string& selection1() const;
        // Return a reference to the non-modifiable "Selection1" selection of
        // this object if "Selection1" is the current selection.  The behavior
        // is undefined unless "Selection1" is the selection of this object.

    const MyEnumeration::Value& selection2() const;
        // Return a reference to the non-modifiable "Selection2" selection of
        // this object if "Selection2" is the current selection.  The behavior
        // is undefined unless "Selection2" is the selection of this object.

    bool isSelection0Value() const;
        // Return 'true' if the value of this object is a "Selection0" value,
        // and return 'false' otherwise.

    bool isSelection1Value() const;
        // Return 'true' if the value of this object is a "Selection1" value,
        // and return 'false' otherwise.

    bool isSelection2Value() const;
        // Return 'true' if the value of this object is a "Selection2" value,
        // and return 'false' otherwise.

    bool isUndefinedValue() const;
        // Return 'true' if the value of this object is undefined, and 'false'
        // otherwise.

    const char *selectionName() const;
        // Return the symbolic name of the current selection of this object.
};

// FREE OPERATORS
inline
bool operator==(const MyChoiceWithDefaultValues& lhs, const MyChoiceWithDefaultValues& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
    // value, and 'false' otherwise.  Two 'MyChoiceWithDefaultValues' objects have the same
    // value if either the selections in both objects have the same ids and
    // the same values, or both selections are undefined.

inline
bool operator!=(const MyChoiceWithDefaultValues& lhs, const MyChoiceWithDefaultValues& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects do not have the
    // same values, as determined by 'operator==', and 'false' otherwise.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MyChoiceWithDefaultValues& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_CHOICE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MyChoiceWithDefaultValues)

namespace test {

                    // ===================================
                    // class MySequenceWithAnonymousChoice
                    // ===================================

class MySequenceWithAnonymousChoice {

    // INSTANCE DATA
    bsl::string                          d_attribute2;
    MySequenceWithAnonymousChoiceChoice  d_choice;
    int                                  d_attribute1;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_ATTRIBUTE1 = 0
      , ATTRIBUTE_ID_CHOICE     = 1
      , ATTRIBUTE_ID_ATTRIBUTE2 = 2
    };

    enum {
        NUM_ATTRIBUTES = 3
    };

    enum {
        ATTRIBUTE_INDEX_ATTRIBUTE1 = 0
      , ATTRIBUTE_INDEX_CHOICE     = 1
      , ATTRIBUTE_INDEX_ATTRIBUTE2 = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequenceWithAnonymousChoice(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithAnonymousChoice' having the
        // default value.  Use the optionally specified 'basicAllocator' to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    MySequenceWithAnonymousChoice(const MySequenceWithAnonymousChoice& original,
                                  bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithAnonymousChoice' having the
        // value of the specified 'original' object.  Use the optionally
        // specified 'basicAllocator' to supply memory.  If 'basicAllocator' is
        // 0, the currently installed default allocator is used.

    ~MySequenceWithAnonymousChoice();
        // Destroy this object.

    // MANIPULATORS
    MySequenceWithAnonymousChoice& operator=(const MySequenceWithAnonymousChoice& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& attribute1();
        // Return a reference to the modifiable "Attribute1" attribute of this
        // object.

    MySequenceWithAnonymousChoiceChoice& choice();
        // Return a reference to the modifiable "Choice" attribute of this
        // object.

    bsl::string& attribute2();
        // Return a reference to the modifiable "Attribute2" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int attribute1() const;
        // Return a reference to the non-modifiable "Attribute1" attribute of
        // this object.

    const MySequenceWithAnonymousChoiceChoice& choice() const;
        // Return a reference to the non-modifiable "Choice" attribute of this
        // object.

    const bsl::string& attribute2() const;
        // Return a reference to the non-modifiable "Attribute2" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequenceWithAnonymousChoice& lhs, const MySequenceWithAnonymousChoice& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const MySequenceWithAnonymousChoice& lhs, const MySequenceWithAnonymousChoice& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequenceWithAnonymousChoice& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequenceWithAnonymousChoice)

namespace test {

                     // =================================
                     // class MySequenceWithDefaultValues
                     // =================================

class MySequenceWithDefaultValues {

    // INSTANCE DATA
    bsl::string           d_attribute1;
    int                   d_attribute0;
    MyEnumeration::Value  d_attribute2;

  public:
    // TYPES
    enum {
        ATTRIBUTE_ID_ATTRIBUTE0 = 0
      , ATTRIBUTE_ID_ATTRIBUTE1 = 1
      , ATTRIBUTE_ID_ATTRIBUTE2 = 2
    };

    enum {
        NUM_ATTRIBUTES = 3
    };

    enum {
        ATTRIBUTE_INDEX_ATTRIBUTE0 = 0
      , ATTRIBUTE_INDEX_ATTRIBUTE1 = 1
      , ATTRIBUTE_INDEX_ATTRIBUTE2 = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const int DEFAULT_INITIALIZER_ATTRIBUTE0;

    static const char DEFAULT_INITIALIZER_ATTRIBUTE1[];

    static const MyEnumeration::Value DEFAULT_INITIALIZER_ATTRIBUTE2;

    static const bdlat_AttributeInfo ATTRIBUTE_INFO_ARRAY[];

  public:
    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_AttributeInfo *lookupAttributeInfo(int id);
        // Return attribute information for the attribute indicated by the
        // specified 'id' if the attribute exists, and 0 otherwise.

    static const bdlat_AttributeInfo *lookupAttributeInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return attribute information for the attribute indicated by the
        // specified 'name' of the specified 'nameLength' if the attribute
        // exists, and 0 otherwise.

    // CREATORS
    explicit MySequenceWithDefaultValues(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithDefaultValues' having the
        // default value.  Use the optionally specified 'basicAllocator' to
        // supply memory.  If 'basicAllocator' is 0, the currently installed
        // default allocator is used.

    MySequenceWithDefaultValues(const MySequenceWithDefaultValues& original,
                                bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'MySequenceWithDefaultValues' having the
        // value of the specified 'original' object.  Use the optionally
        // specified 'basicAllocator' to supply memory.  If 'basicAllocator' is
        // 0, the currently installed default allocator is used.

    ~MySequenceWithDefaultValues();
        // Destroy this object.

    // MANIPULATORS
    MySequenceWithDefaultValues& operator=(const MySequenceWithDefaultValues& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon
        // default construction).

    template<class MANIPULATOR>
    int manipulateAttributes(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' sequentially on the address of
        // each (modifiable) attribute of this object, supplying 'manipulator'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'manipulator' (i.e., the invocation that
        // terminated the sequence).

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR& manipulator, int id);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'id',
        // supplying 'manipulator' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if 'id' identifies an attribute of this
        // class, and -1 otherwise.

    template<class MANIPULATOR>
    int manipulateAttribute(MANIPULATOR&  manipulator,
                            const char   *name,
                            int           nameLength);
        // Invoke the specified 'manipulator' on the address of
        // the (modifiable) attribute indicated by the specified 'name' of the
        // specified 'nameLength', supplying 'manipulator' with the
        // corresponding attribute information structure.  Return the value
        // returned from the invocation of 'manipulator' if 'name' identifies
        // an attribute of this class, and -1 otherwise.

    int& attribute0();
        // Return a reference to the modifiable "Attribute0" attribute of this
        // object.

    bsl::string& attribute1();
        // Return a reference to the modifiable "Attribute1" attribute of this
        // object.

    MyEnumeration::Value& attribute2();
        // Return a reference to the modifiable "Attribute2" attribute of this
        // object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    template<class ACCESSOR>
    int accessAttributes(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' sequentially on each
        // (non-modifiable) attribute of this object, supplying 'accessor'
        // with the corresponding attribute information structure until such
        // invocation returns a non-zero value.  Return the value from the
        // last invocation of 'accessor' (i.e., the invocation that terminated
        // the sequence).

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR& accessor, int id) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'id', supplying 'accessor'
        // with the corresponding attribute information structure.  Return the
        // value returned from the invocation of 'accessor' if 'id' identifies
        // an attribute of this class, and -1 otherwise.

    template<class ACCESSOR>
    int accessAttribute(ACCESSOR&   accessor,
                        const char *name,
                        int         nameLength) const;
        // Invoke the specified 'accessor' on the (non-modifiable) attribute
        // of this object indicated by the specified 'name' of the specified
        // 'nameLength', supplying 'accessor' with the corresponding attribute
        // information structure.  Return the value returned from the
        // invocation of 'accessor' if 'name' identifies an attribute of this
        // class, and -1 otherwise.

    int attribute0() const;
        // Return a reference to the non-modifiable "Attribute0" attribute of
        // this object.

    const bsl::string& attribute1() const;
        // Return a reference to the non-modifiable "Attribute1" attribute of
        // this object.

    MyEnumeration::Value attribute2() const;
        // Return a reference to the non-modifiable "Attribute2" attribute of
        // this object.
};

// FREE OPERATORS
inline
bool operator==(const MySequenceWithDefaultValues& lhs, const MySequenceWithDefaultValues& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects have
    // the same value, and 'false' otherwise.  Two attribute objects have the
    // same value if each respective attribute has the same value.

inline
bool operator!=(const MySequenceWithDefaultValues& lhs, const MySequenceWithDefaultValues& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' attribute objects do not
    // have the same value, and 'false' otherwise.  Two attribute objects do
    // not have the same value if one or more respective attributes differ in
    // values.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const MySequenceWithDefaultValues& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_SEQUENCE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::MySequenceWithDefaultValues)

namespace test {

                            // ===================
                            // class TimingRequest
                            // ===================

class TimingRequest {
    // A choice record representative of a typical request object.

    // INSTANCE DATA
    union {
        bsls::ObjectBuffer< Sqrt >        d_sqrt;
        bsls::ObjectBuffer< BasicRecord > d_basic;
        bsls::ObjectBuffer< BigRecord >   d_big;
    };

    int                                   d_selectionId;
    bslma::Allocator                     *d_allocator_p;

  public:
    // TYPES

    enum {
        SELECTION_ID_UNDEFINED = -1
      , SELECTION_ID_SQRT  = 0
      , SELECTION_ID_BASIC = 1
      , SELECTION_ID_BIG   = 2
    };

    enum {
        NUM_SELECTIONS = 3
    };

    enum {
        SELECTION_INDEX_SQRT  = 0
      , SELECTION_INDEX_BASIC = 1
      , SELECTION_INDEX_BIG   = 2
    };

    // CONSTANTS
    static const char CLASS_NAME[];

    static const bdlat_SelectionInfo SELECTION_INFO_ARRAY[];

    // CLASS METHODS
    static int maxSupportedBdexVersion();
        // Return the most current 'bdex' streaming version number supported by
        // this class.  See the 'bslx' package-level documentation for more
        // information on 'bdex' streaming of value-semantic types and
        // containers.

    static const bdlat_SelectionInfo *lookupSelectionInfo(int id);
        // Return selection information for the selection indicated by the
        // specified 'id' if the selection exists, and 0 otherwise.

    static const bdlat_SelectionInfo *lookupSelectionInfo(
                                                       const char *name,
                                                       int         nameLength);
        // Return selection information for the selection indicated by the
        // specified 'name' of the specified 'nameLength' if the selection
        // exists, and 0 otherwise.

    // CREATORS
    explicit TimingRequest(bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'TimingRequest' having the default value.
        // Use the optionally specified 'basicAllocator' to supply memory.  If
        // 'basicAllocator' is 0, the currently installed default allocator is
        // used.

    TimingRequest(const TimingRequest& original,
                 bslma::Allocator *basicAllocator = 0);
        // Create an object of type 'TimingRequest' having the value of the
        // specified 'original' object.  Use the optionally specified
        // 'basicAllocator' to supply memory.  If 'basicAllocator' is 0, the
        // currently installed default allocator is used.

    ~TimingRequest();
        // Destroy this object.

    // MANIPULATORS
    TimingRequest& operator=(const TimingRequest& rhs);
        // Assign to this object the value of the specified 'rhs' object.

    template <class STREAM>
    STREAM& bdexStreamIn(STREAM& stream, int version);
        // Assign to this object the value read from the specified input
        // 'stream' using the specified 'version' format and return a reference
        // to the modifiable 'stream'.  If 'stream' is initially invalid, this
        // operation has no effect.  If 'stream' becomes invalid during this
        // operation, this object is valid, but its value is undefined.  If
        // 'version' is not supported, 'stream' is marked invalid and this
        // object is unaltered.  Note that no version is read from 'stream'.
        // See the 'bslx' package-level documentation for more information on
        // 'bdex' streaming of value-semantic types and containers.

    void reset();
        // Reset this object to the default value (i.e., its value upon default
        // construction).

    int makeSelection(int selectionId);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'selectionId'.  Return 0 on success, and
        // non-zero value otherwise (i.e., the selection is not found).

    int makeSelection(const char *name, int nameLength);
        // Set the value of this object to be the default for the selection
        // indicated by the specified 'name' of the specified 'nameLength'.
        // Return 0 on success, and non-zero value otherwise (i.e., the
        // selection is not found).

    Sqrt& makeSqrt();
    Sqrt& makeSqrt(const Sqrt& value);
        // Set the value of this object to be a "Sqrt" value.  Optionally
        // specify the 'value' of the "Sqrt".  If 'value' is not specified, the
        // default "Sqrt" value is used.

    BasicRecord& makeBasic();
    BasicRecord& makeBasic(const BasicRecord& value);
        // Set the value of this object to be a "Basic" value.  Optionally
        // specify the 'value' of the "Basic".  If 'value' is not specified,
        // the default "Basic" value is used.

    BigRecord& makeBig();
    BigRecord& makeBig(const BigRecord& value);
        // Set the value of this object to be a "Big" value.  Optionally
        // specify the 'value' of the "Big".  If 'value' is not specified, the
        // default "Big" value is used.

    template<class MANIPULATOR>
    int manipulateSelection(MANIPULATOR& manipulator);
        // Invoke the specified 'manipulator' on the address of the modifiable
        // selection, supplying 'manipulator' with the corresponding selection
        // information structure.  Return the value returned from the
        // invocation of 'manipulator' if this object has a defined selection,
        // and -1 otherwise.

    Sqrt& sqrt();
        // Return a reference to the modifiable "Sqrt" selection of this object
        // if "Sqrt" is the current selection.  The behavior is undefined
        // unless "Sqrt" is the selection of this object.

    BasicRecord& basic();
        // Return a reference to the modifiable "Basic" selection of this
        // object if "Basic" is the current selection.  The behavior is
        // undefined unless "Basic" is the selection of this object.

    BigRecord& big();
        // Return a reference to the modifiable "Big" selection of this object
        // if "Big" is the current selection.  The behavior is undefined unless
        // "Big" is the selection of this object.

    // ACCESSORS
    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the
        // optionally specified indentation 'level' and return a reference to
        // the modifiable 'stream'.  If 'level' is specified, optionally
        // specify 'spacesPerLevel', the number of spaces per indentation level
        // for this and all of its nested objects.  Each line is indented by
        // the absolute value of 'level * spacesPerLevel'.  If 'level' is
        // negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, suppress line breaks and format the
        // entire output on one line.  If 'stream' is initially invalid, this
        // operation has no effect.  Note that a trailing newline is provided
        // in multiline mode only.

    template <class STREAM>
    STREAM& bdexStreamOut(STREAM& stream, int version) const;
        // Write the value of this object to the specified output 'stream'
        // using the specified 'version' format and return a reference to the
        // modifiable 'stream'.  If 'version' is not supported, 'stream' is
        // unmodified.  Note that 'version' is not written to 'stream'.
        // See the 'bslx' package-level documentation for more information
        // on 'bdex' streaming of value-semantic types and containers.

    int selectionId() const;
        // Return the id of the current selection if the selection is defined,
        // and -1 otherwise.

    template<class ACCESSOR>
    int accessSelection(ACCESSOR& accessor) const;
        // Invoke the specified 'accessor' on the non-modifiable selection,
        // supplying 'accessor' with the corresponding selection information
        // structure.  Return the value returned from the invocation of
        // 'accessor' if this object has a defined selection, and -1 otherwise.

    const Sqrt& sqrt() const;
        // Return a reference to the non-modifiable "Sqrt" selection of this
        // object if "Sqrt" is the current selection.  The behavior is
        // undefined unless "Sqrt" is the selection of this object.

    const BasicRecord& basic() const;
        // Return a reference to the non-modifiable "Basic" selection of this
        // object if "Basic" is the current selection.  The behavior is
        // undefined unless "Basic" is the selection of this object.

    const BigRecord& big() const;
        // Return a reference to the non-modifiable "Big" selection of this
        // object if "Big" is the current selection.  The behavior is undefined
        // unless "Big" is the selection of this object.

    bool isSqrtValue() const;
        // Return 'true' if the value of this object is a "Sqrt" value, and
        // return 'false' otherwise.

    bool isBasicValue() const;
        // Return 'true' if the value of this object is a "Basic" value, and
        // return 'false' otherwise.

    bool isBigValue() const;
        // Return 'true' if the value of this object is a "Big" value, and
        // return 'false' otherwise.

    bool isUndefinedValue() const;
        // Return 'true' if the value of this object is undefined, and 'false'
        // otherwise.

    const char *selectionName() const;
        // Return the symbolic name of the current selection of this object.
};

// FREE OPERATORS
inline
bool operator==(const TimingRequest& lhs, const TimingRequest& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
    // value, and 'false' otherwise.  Two 'TimingRequest' objects have the same
    // value if either the selections in both objects have the same ids and
    // the same values, or both selections are undefined.

inline
bool operator!=(const TimingRequest& lhs, const TimingRequest& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects do not have the
    // same values, as determined by 'operator==', and 'false' otherwise.

inline
bsl::ostream& operator<<(bsl::ostream& stream, const TimingRequest& rhs);
    // Format the specified 'rhs' to the specified output 'stream' and
    // return a reference to the modifiable 'stream'.

}  // close package namespace

// TRAITS

BDLAT_DECL_CHOICE_WITH_ALLOCATOR_BITWISEMOVEABLE_TRAITS(test::TimingRequest)

// ============================================================================
//                         INLINE FUNCTION DEFINITIONS
// ============================================================================

namespace test {

                               // -------------
                               // class Address
                               // -------------

// CLASS METHODS
inline
int Address::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& Address::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_street, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_city, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_state, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int Address::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_street, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STREET]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_city, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CITY]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_state, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STATE]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int Address::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_STREET: {
        return manipulator(&d_street, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STREET]);
      } break;
      case ATTRIBUTE_ID_CITY: {
        return manipulator(&d_city, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CITY]);
      } break;
      case ATTRIBUTE_ID_STATE: {
        return manipulator(&d_state, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STATE]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int Address::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
bsl::string& Address::street()
{
    return d_street;
}

inline
bsl::string& Address::city()
{
    return d_city;
}

inline
bsl::string& Address::state()
{
    return d_state;
}

// ACCESSORS
template <class STREAM>
STREAM& Address::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_street, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_city, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_state, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int Address::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_street, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STREET]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_city, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CITY]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_state, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STATE]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int Address::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_STREET: {
        return accessor(d_street, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STREET]);
      } break;
      case ATTRIBUTE_ID_CITY: {
        return accessor(d_city, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CITY]);
      } break;
      case ATTRIBUTE_ID_STATE: {
        return accessor(d_state, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STATE]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int Address::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
const bsl::string& Address::street() const
{
    return d_street;
}

inline
const bsl::string& Address::city() const
{
    return d_city;
}

inline
const bsl::string& Address::state() const
{
    return d_state;
}



                             // -----------------
                             // class BasicRecord
                             // -----------------

// CLASS METHODS
inline
int BasicRecord::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& BasicRecord::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_i1, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_i2, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_dt, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_s, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int BasicRecord::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_i1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_i2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I2]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_dt, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_DT]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_s, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_S]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int BasicRecord::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_I1: {
        return manipulator(&d_i1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I1]);
      } break;
      case ATTRIBUTE_ID_I2: {
        return manipulator(&d_i2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I2]);
      } break;
      case ATTRIBUTE_ID_DT: {
        return manipulator(&d_dt, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_DT]);
      } break;
      case ATTRIBUTE_ID_S: {
        return manipulator(&d_s, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_S]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int BasicRecord::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& BasicRecord::i1()
{
    return d_i1;
}

inline
int& BasicRecord::i2()
{
    return d_i2;
}

inline
bdlt::DatetimeTz& BasicRecord::dt()
{
    return d_dt;
}

inline
bsl::string& BasicRecord::s()
{
    return d_s;
}

// ACCESSORS
template <class STREAM>
STREAM& BasicRecord::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_i1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_i2, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_dt, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_s, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int BasicRecord::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_i1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_i2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I2]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_dt, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_DT]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_s, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_S]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int BasicRecord::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_I1: {
        return accessor(d_i1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I1]);
      } break;
      case ATTRIBUTE_ID_I2: {
        return accessor(d_i2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I2]);
      } break;
      case ATTRIBUTE_ID_DT: {
        return accessor(d_dt, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_DT]);
      } break;
      case ATTRIBUTE_ID_S: {
        return accessor(d_s, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_S]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int BasicRecord::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int BasicRecord::i1() const
{
    return d_i1;
}

inline
int BasicRecord::i2() const
{
    return d_i2;
}

inline
const bdlt::DatetimeTz& BasicRecord::dt() const
{
    return d_dt;
}

inline
const bsl::string& BasicRecord::s() const
{
    return d_s;
}



                           // ----------------------
                           // class CustomizedString
                           // ----------------------

// CREATORS
inline
CustomizedString::CustomizedString(bslma::Allocator *basicAllocator)
: d_value(basicAllocator)
{
}

inline
CustomizedString::CustomizedString(const CustomizedString& original, bslma::Allocator *basicAllocator)
: d_value(original.d_value, basicAllocator)
{
}

inline
CustomizedString::CustomizedString(const bsl::string& value, bslma::Allocator *basicAllocator)
: d_value(value, basicAllocator)
{
    BSLS_ASSERT(checkRestrictions(value) == 0);
}

inline
CustomizedString::~CustomizedString()
{
}

// MANIPULATORS
inline
CustomizedString& CustomizedString::operator=(const CustomizedString& rhs)
{
    d_value = rhs.d_value;
    return *this;
}

template <class STREAM>
STREAM& CustomizedString::bdexStreamIn(STREAM& stream, int version)
{
    bsl::string temp;

    bslx::InStreamFunctions::bdexStreamIn(stream, temp, version);

    if (!stream) {
        return stream;
    }

    if (fromString(temp)!=0) {
        stream.invalidate();
    }

    return stream;
}

inline
void CustomizedString::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_value);
}

inline
int CustomizedString::fromString(const bsl::string& value)
{
    int ret = checkRestrictions(value);
    if (0 == ret) {
        d_value = value;
    }

    return ret;
}

// ACCESSORS
template <class STREAM>
STREAM& CustomizedString::bdexStreamOut(STREAM& stream, int version) const
{
    return bslx::OutStreamFunctions::bdexStreamOut(stream, d_value, version);
}

inline
int CustomizedString::maxSupportedBdexVersion() const
{
    return bslx::VersionFunctions::maxSupportedBdexVersion(&d_value, 20171121);
}

inline
bsl::ostream& CustomizedString::print(bsl::ostream& stream,
                                 int           level,
                                 int           spacesPerLevel) const
{
    return bdlb::PrintMethods::print(stream, d_value, level, spacesPerLevel);
}

inline
const bsl::string& CustomizedString::toString() const
{
    return d_value;
}



                               // --------------
                               // class MyChoice
                               // --------------

// CLASS METHODS
inline
int MyChoice::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// CREATORS
inline
MyChoice::MyChoice(bslma::Allocator *basicAllocator)
: d_selectionId(SELECTION_ID_UNDEFINED)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

inline
MyChoice::~MyChoice()
{
    reset();
}

// MANIPULATORS
template <class STREAM>
STREAM& MyChoice::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            short selectionId;
            stream.getInt16(selectionId);
            if (!stream) {
                return stream;
            }
            switch (selectionId) {
              case SELECTION_ID_SELECTION1: {
                makeSelection1();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_selection1.object(), 1);
              } break;
              case SELECTION_ID_SELECTION2: {
                makeSelection2();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_selection2.object(), 1);
              } break;
              case SELECTION_ID_UNDEFINED: {
                reset();
              } break;
              default:
                stream.invalidate();
            }
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MyChoice::manipulateSelection(MANIPULATOR& manipulator)
{
    switch (d_selectionId) {
      case MyChoice::SELECTION_ID_SELECTION1:
        return manipulator(&d_selection1.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1]);
      case MyChoice::SELECTION_ID_SELECTION2:
        return manipulator(&d_selection2.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2]);
      default:
        BSLS_ASSERT(MyChoice::SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
int& MyChoice::selection1()
{
    BSLS_ASSERT(SELECTION_ID_SELECTION1 == d_selectionId);
    return d_selection1.object();
}

inline
bsl::string& MyChoice::selection2()
{
    BSLS_ASSERT(SELECTION_ID_SELECTION2 == d_selectionId);
    return d_selection2.object();
}

// ACCESSORS
template <class STREAM>
STREAM& MyChoice::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
            stream.putInt16(d_selectionId);
            switch (d_selectionId) {
              case SELECTION_ID_SELECTION1: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_selection1.object(), 1);
              } break;
              case SELECTION_ID_SELECTION2: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_selection2.object(), 1);
              } break;
              default:
                BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
            }
      } break;
    }
    return stream;
}

inline
int MyChoice::selectionId() const
{
    return d_selectionId;
}

template <class ACCESSOR>
int MyChoice::accessSelection(ACCESSOR& accessor) const
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION1:
        return accessor(d_selection1.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1]);
      case SELECTION_ID_SELECTION2:
        return accessor(d_selection2.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2]);
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
const int& MyChoice::selection1() const
{
    BSLS_ASSERT(SELECTION_ID_SELECTION1 == d_selectionId);
    return d_selection1.object();
}

inline
const bsl::string& MyChoice::selection2() const
{
    BSLS_ASSERT(SELECTION_ID_SELECTION2 == d_selectionId);
    return d_selection2.object();
}

inline
bool MyChoice::isSelection1Value() const
{
    return SELECTION_ID_SELECTION1 == d_selectionId;
}

inline
bool MyChoice::isSelection2Value() const
{
    return SELECTION_ID_SELECTION2 == d_selectionId;
}

inline
bool MyChoice::isUndefinedValue() const
{
    return SELECTION_ID_UNDEFINED == d_selectionId;
}


                            // -------------------
                            // class MyEnumeration
                            // -------------------

// CLASS METHODS
inline
int MyEnumeration::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1
}

inline
int MyEnumeration::fromString(Value *result, const bsl::string& string)
{
    return fromString(result, string.c_str(), static_cast<int>(string.length()));
}

inline
bsl::ostream& MyEnumeration::print(bsl::ostream&      stream,
                                 MyEnumeration::Value value)
{
    return stream << toString(value);
}

template <class STREAM>
STREAM& MyEnumeration::bdexStreamIn(STREAM&             stream,
                                   MyEnumeration::Value& value,
                                   int                 version)
{
    switch(version) {
      case 1: {
        int readValue;
        stream.getInt32(readValue);
        if (stream) {
            if (fromInt(&value, readValue)) {
               stream.invalidate();   // bad value in stream
            }
        }
      } break;
      default: {
        stream.invalidate();          // unrecognized version number
      } break;
    }
    return stream;
}

template <class STREAM>
STREAM& MyEnumeration::bdexStreamOut(STREAM&              stream,
                                    MyEnumeration::Value value,
                                    int                version)
{
    switch (version) {
      case 1: {
        stream.putInt32(value);  // Write the value as an int
      } break;
    }
    return stream;
}



                              // ----------------
                              // class MySequence
                              // ----------------

// CLASS METHODS
inline
int MySequence::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequence::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute1, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute2, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequence::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int MySequence::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int MySequence::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& MySequence::attribute1()
{
    return d_attribute1;
}

inline
bsl::string& MySequence::attribute2()
{
    return d_attribute2;
}

// ACCESSORS
template <class STREAM>
STREAM& MySequence::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute2, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int MySequence::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int MySequence::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int MySequence::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int MySequence::attribute1() const
{
    return d_attribute1;
}

inline
const bsl::string& MySequence::attribute2() const
{
    return d_attribute2;
}



                 // -----------------------------------------
                 // class MySequenceWithAnonymousChoiceChoice
                 // -----------------------------------------

// CLASS METHODS
inline
int MySequenceWithAnonymousChoiceChoice::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// CREATORS
inline
MySequenceWithAnonymousChoiceChoice::MySequenceWithAnonymousChoiceChoice(bslma::Allocator *basicAllocator)
: d_selectionId(SELECTION_ID_UNDEFINED)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

inline
MySequenceWithAnonymousChoiceChoice::~MySequenceWithAnonymousChoiceChoice()
{
    reset();
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequenceWithAnonymousChoiceChoice::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            short selectionId;
            stream.getInt16(selectionId);
            if (!stream) {
                return stream;
            }
            switch (selectionId) {
              case SELECTION_ID_MY_CHOICE1: {
                makeMyChoice1();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_myChoice1.object(), 1);
              } break;
              case SELECTION_ID_MY_CHOICE2: {
                makeMyChoice2();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_myChoice2.object(), 1);
              } break;
              case SELECTION_ID_UNDEFINED: {
                reset();
              } break;
              default:
                stream.invalidate();
            }
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequenceWithAnonymousChoiceChoice::manipulateSelection(MANIPULATOR& manipulator)
{
    switch (d_selectionId) {
      case MySequenceWithAnonymousChoiceChoice::SELECTION_ID_MY_CHOICE1:
        return manipulator(&d_myChoice1.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE1]);
      case MySequenceWithAnonymousChoiceChoice::SELECTION_ID_MY_CHOICE2:
        return manipulator(&d_myChoice2.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE2]);
      default:
        BSLS_ASSERT(MySequenceWithAnonymousChoiceChoice::SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
int& MySequenceWithAnonymousChoiceChoice::myChoice1()
{
    BSLS_ASSERT(SELECTION_ID_MY_CHOICE1 == d_selectionId);
    return d_myChoice1.object();
}

inline
bsl::string& MySequenceWithAnonymousChoiceChoice::myChoice2()
{
    BSLS_ASSERT(SELECTION_ID_MY_CHOICE2 == d_selectionId);
    return d_myChoice2.object();
}

// ACCESSORS
template <class STREAM>
STREAM& MySequenceWithAnonymousChoiceChoice::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
            stream.putInt16(d_selectionId);
            switch (d_selectionId) {
              case SELECTION_ID_MY_CHOICE1: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_myChoice1.object(), 1);
              } break;
              case SELECTION_ID_MY_CHOICE2: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_myChoice2.object(), 1);
              } break;
              default:
                BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
            }
      } break;
    }
    return stream;
}

inline
int MySequenceWithAnonymousChoiceChoice::selectionId() const
{
    return d_selectionId;
}

template <class ACCESSOR>
int MySequenceWithAnonymousChoiceChoice::accessSelection(ACCESSOR& accessor) const
{
    switch (d_selectionId) {
      case SELECTION_ID_MY_CHOICE1:
        return accessor(d_myChoice1.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE1]);
      case SELECTION_ID_MY_CHOICE2:
        return accessor(d_myChoice2.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE2]);
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
const int& MySequenceWithAnonymousChoiceChoice::myChoice1() const
{
    BSLS_ASSERT(SELECTION_ID_MY_CHOICE1 == d_selectionId);
    return d_myChoice1.object();
}

inline
const bsl::string& MySequenceWithAnonymousChoiceChoice::myChoice2() const
{
    BSLS_ASSERT(SELECTION_ID_MY_CHOICE2 == d_selectionId);
    return d_myChoice2.object();
}

inline
bool MySequenceWithAnonymousChoiceChoice::isMyChoice1Value() const
{
    return SELECTION_ID_MY_CHOICE1 == d_selectionId;
}

inline
bool MySequenceWithAnonymousChoiceChoice::isMyChoice2Value() const
{
    return SELECTION_ID_MY_CHOICE2 == d_selectionId;
}

inline
bool MySequenceWithAnonymousChoiceChoice::isUndefinedValue() const
{
    return SELECTION_ID_UNDEFINED == d_selectionId;
}


                         // -------------------------
                         // class MySequenceWithArray
                         // -------------------------

// CLASS METHODS
inline
int MySequenceWithArray::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequenceWithArray::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute1, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute2, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequenceWithArray::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int MySequenceWithArray::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int MySequenceWithArray::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& MySequenceWithArray::attribute1()
{
    return d_attribute1;
}

inline
bsl::vector<bsl::string>& MySequenceWithArray::attribute2()
{
    return d_attribute2;
}

// ACCESSORS
template <class STREAM>
STREAM& MySequenceWithArray::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute2, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int MySequenceWithArray::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int MySequenceWithArray::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int MySequenceWithArray::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int MySequenceWithArray::attribute1() const
{
    return d_attribute1;
}

inline
const bsl::vector<bsl::string>& MySequenceWithArray::attribute2() const
{
    return d_attribute2;
}



                        // ----------------------------
                        // class MySequenceWithNillable
                        // ----------------------------

// CLASS METHODS
inline
int MySequenceWithNillable::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequenceWithNillable::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute1, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_myNillable, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute2, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequenceWithNillable::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_myNillable, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_MY_NILLABLE]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int MySequenceWithNillable::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_MY_NILLABLE: {
        return manipulator(&d_myNillable, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_MY_NILLABLE]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int MySequenceWithNillable::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& MySequenceWithNillable::attribute1()
{
    return d_attribute1;
}

inline
bdlb::NullableValue<bsl::string>& MySequenceWithNillable::myNillable()
{
    return d_myNillable;
}

inline
bsl::string& MySequenceWithNillable::attribute2()
{
    return d_attribute2;
}

// ACCESSORS
template <class STREAM>
STREAM& MySequenceWithNillable::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_myNillable, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute2, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int MySequenceWithNillable::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_myNillable, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_MY_NILLABLE]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int MySequenceWithNillable::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_MY_NILLABLE: {
        return accessor(d_myNillable, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_MY_NILLABLE]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int MySequenceWithNillable::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int MySequenceWithNillable::attribute1() const
{
    return d_attribute1;
}

inline
const bdlb::NullableValue<bsl::string>& MySequenceWithNillable::myNillable() const
{
    return d_myNillable;
}

inline
const bsl::string& MySequenceWithNillable::attribute2() const
{
    return d_attribute2;
}



                        // ----------------------------
                        // class MySequenceWithNullable
                        // ----------------------------

// CLASS METHODS
inline
int MySequenceWithNullable::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequenceWithNullable::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute1, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute2, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequenceWithNullable::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int MySequenceWithNullable::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int MySequenceWithNullable::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& MySequenceWithNullable::attribute1()
{
    return d_attribute1;
}

inline
bdlb::NullableValue<bsl::string>& MySequenceWithNullable::attribute2()
{
    return d_attribute2;
}

// ACCESSORS
template <class STREAM>
STREAM& MySequenceWithNullable::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute2, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int MySequenceWithNullable::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int MySequenceWithNullable::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int MySequenceWithNullable::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int MySequenceWithNullable::attribute1() const
{
    return d_attribute1;
}

inline
const bdlb::NullableValue<bsl::string>& MySequenceWithNullable::attribute2() const
{
    return d_attribute2;
}



                               // -------------
                               // class RawData
                               // -------------

// CLASS METHODS
inline
int RawData::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& RawData::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_charvec, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_ucharvec, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int RawData::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int RawData::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_CHARVEC: {
        return manipulator(&d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
      } break;
      case ATTRIBUTE_ID_UCHARVEC: {
        return manipulator(&d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int RawData::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
bsl::vector<char>& RawData::charvec()
{
    return d_charvec;
}

inline
bsl::vector<unsigned char>& RawData::ucharvec()
{
    return d_ucharvec;
}

// ACCESSORS
template <class STREAM>
STREAM& RawData::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_charvec, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_ucharvec, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int RawData::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int RawData::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_CHARVEC: {
        return accessor(d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
      } break;
      case ATTRIBUTE_ID_UCHARVEC: {
        return accessor(d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int RawData::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
const bsl::vector<char>& RawData::charvec() const
{
    return d_charvec;
}

inline
const bsl::vector<unsigned char>& RawData::ucharvec() const
{
    return d_ucharvec;
}



                           // ---------------------
                           // class RawDataSwitched
                           // ---------------------

// CLASS METHODS
inline
int RawDataSwitched::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& RawDataSwitched::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_charvec, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_ucharvec, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int RawDataSwitched::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int RawDataSwitched::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_CHARVEC: {
        return manipulator(&d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
      } break;
      case ATTRIBUTE_ID_UCHARVEC: {
        return manipulator(&d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int RawDataSwitched::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
bsl::vector<char>& RawDataSwitched::charvec()
{
    return d_charvec;
}

inline
bsl::vector<unsigned char>& RawDataSwitched::ucharvec()
{
    return d_ucharvec;
}

// ACCESSORS
template <class STREAM>
STREAM& RawDataSwitched::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_charvec, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_ucharvec, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int RawDataSwitched::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int RawDataSwitched::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_CHARVEC: {
        return accessor(d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
      } break;
      case ATTRIBUTE_ID_UCHARVEC: {
        return accessor(d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int RawDataSwitched::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
const bsl::vector<char>& RawDataSwitched::charvec() const
{
    return d_charvec;
}

inline
const bsl::vector<unsigned char>& RawDataSwitched::ucharvec() const
{
    return d_ucharvec;
}



                          // ------------------------
                          // class RawDataUnformatted
                          // ------------------------

// CLASS METHODS
inline
int RawDataUnformatted::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& RawDataUnformatted::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_charvec, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_ucharvec, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int RawDataUnformatted::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int RawDataUnformatted::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_CHARVEC: {
        return manipulator(&d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
      } break;
      case ATTRIBUTE_ID_UCHARVEC: {
        return manipulator(&d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int RawDataUnformatted::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
bsl::vector<char>& RawDataUnformatted::charvec()
{
    return d_charvec;
}

inline
bsl::vector<unsigned char>& RawDataUnformatted::ucharvec()
{
    return d_ucharvec;
}

// ACCESSORS
template <class STREAM>
STREAM& RawDataUnformatted::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_charvec, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_ucharvec, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int RawDataUnformatted::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int RawDataUnformatted::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_CHARVEC: {
        return accessor(d_charvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC]);
      } break;
      case ATTRIBUTE_ID_UCHARVEC: {
        return accessor(d_ucharvec, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int RawDataUnformatted::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
const bsl::vector<char>& RawDataUnformatted::charvec() const
{
    return d_charvec;
}

inline
const bsl::vector<unsigned char>& RawDataUnformatted::ucharvec() const
{
    return d_ucharvec;
}



                                 // ----------
                                 // class Sqrt
                                 // ----------

// CLASS METHODS
inline
int Sqrt::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& Sqrt::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_value, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int Sqrt::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_value, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_VALUE]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int Sqrt::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_VALUE: {
        return manipulator(&d_value, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_VALUE]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int Sqrt::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
double& Sqrt::value()
{
    return d_value;
}

// ACCESSORS
template <class STREAM>
STREAM& Sqrt::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_value, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int Sqrt::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_value, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_VALUE]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int Sqrt::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_VALUE: {
        return accessor(d_value, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_VALUE]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int Sqrt::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
double Sqrt::value() const
{
    return d_value;
}



                              // ---------------
                              // class BigRecord
                              // ---------------

// CLASS METHODS
inline
int BigRecord::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& BigRecord::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_name, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_array, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int BigRecord::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_array, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ARRAY]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int BigRecord::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_NAME: {
        return manipulator(&d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
      } break;
      case ATTRIBUTE_ID_ARRAY: {
        return manipulator(&d_array, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ARRAY]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int BigRecord::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
bsl::string& BigRecord::name()
{
    return d_name;
}

inline
bsl::vector<BasicRecord>& BigRecord::array()
{
    return d_array;
}

// ACCESSORS
template <class STREAM>
STREAM& BigRecord::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_name, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_array, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int BigRecord::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_array, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ARRAY]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int BigRecord::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_NAME: {
        return accessor(d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
      } break;
      case ATTRIBUTE_ID_ARRAY: {
        return accessor(d_array, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ARRAY]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int BigRecord::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
const bsl::string& BigRecord::name() const
{
    return d_name;
}

inline
const bsl::vector<BasicRecord>& BigRecord::array() const
{
    return d_array;
}



                               // --------------
                               // class Employee
                               // --------------

// CLASS METHODS
inline
int Employee::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& Employee::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_name, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_homeAddress, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_age, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int Employee::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_homeAddress, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_HOME_ADDRESS]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_age, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_AGE]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int Employee::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_NAME: {
        return manipulator(&d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
      } break;
      case ATTRIBUTE_ID_HOME_ADDRESS: {
        return manipulator(&d_homeAddress, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_HOME_ADDRESS]);
      } break;
      case ATTRIBUTE_ID_AGE: {
        return manipulator(&d_age, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_AGE]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int Employee::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
bsl::string& Employee::name()
{
    return d_name;
}

inline
Address& Employee::homeAddress()
{
    return d_homeAddress;
}

inline
int& Employee::age()
{
    return d_age;
}

// ACCESSORS
template <class STREAM>
STREAM& Employee::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_name, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_homeAddress, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_age, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int Employee::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_homeAddress, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_HOME_ADDRESS]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_age, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_AGE]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int Employee::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_NAME: {
        return accessor(d_name, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME]);
      } break;
      case ATTRIBUTE_ID_HOME_ADDRESS: {
        return accessor(d_homeAddress, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_HOME_ADDRESS]);
      } break;
      case ATTRIBUTE_ID_AGE: {
        return accessor(d_age, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_AGE]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int Employee::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
const bsl::string& Employee::name() const
{
    return d_name;
}

inline
const Address& Employee::homeAddress() const
{
    return d_homeAddress;
}

inline
int Employee::age() const
{
    return d_age;
}



                      // -------------------------------
                      // class MyChoiceWithDefaultValues
                      // -------------------------------

// CLASS METHODS
inline
int MyChoiceWithDefaultValues::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// CREATORS
inline
MyChoiceWithDefaultValues::MyChoiceWithDefaultValues(bslma::Allocator *basicAllocator)
: d_selectionId(SELECTION_ID_UNDEFINED)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

inline
MyChoiceWithDefaultValues::~MyChoiceWithDefaultValues()
{
    reset();
}

// MANIPULATORS
template <class STREAM>
STREAM& MyChoiceWithDefaultValues::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            short selectionId;
            stream.getInt16(selectionId);
            if (!stream) {
                return stream;
            }
            switch (selectionId) {
              case SELECTION_ID_SELECTION0: {
                makeSelection0();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_selection0.object(), 1);
              } break;
              case SELECTION_ID_SELECTION1: {
                makeSelection1();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_selection1.object(), 1);
              } break;
              case SELECTION_ID_SELECTION2: {
                makeSelection2();
                MyEnumeration::bdexStreamIn(
                    stream, d_selection2.object(), 1);
              } break;
              case SELECTION_ID_UNDEFINED: {
                reset();
              } break;
              default:
                stream.invalidate();
            }
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MyChoiceWithDefaultValues::manipulateSelection(MANIPULATOR& manipulator)
{
    switch (d_selectionId) {
      case MyChoiceWithDefaultValues::SELECTION_ID_SELECTION0:
        return manipulator(&d_selection0.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION0]);
      case MyChoiceWithDefaultValues::SELECTION_ID_SELECTION1:
        return manipulator(&d_selection1.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1]);
      case MyChoiceWithDefaultValues::SELECTION_ID_SELECTION2:
        return manipulator(&d_selection2.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2]);
      default:
        BSLS_ASSERT(MyChoiceWithDefaultValues::SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
int& MyChoiceWithDefaultValues::selection0()
{
    BSLS_ASSERT(SELECTION_ID_SELECTION0 == d_selectionId);
    return d_selection0.object();
}

inline
bsl::string& MyChoiceWithDefaultValues::selection1()
{
    BSLS_ASSERT(SELECTION_ID_SELECTION1 == d_selectionId);
    return d_selection1.object();
}

inline
MyEnumeration::Value& MyChoiceWithDefaultValues::selection2()
{
    BSLS_ASSERT(SELECTION_ID_SELECTION2 == d_selectionId);
    return d_selection2.object();
}

// ACCESSORS
template <class STREAM>
STREAM& MyChoiceWithDefaultValues::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
            stream.putInt16(d_selectionId);
            switch (d_selectionId) {
              case SELECTION_ID_SELECTION0: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_selection0.object(), 1);
              } break;
              case SELECTION_ID_SELECTION1: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_selection1.object(), 1);
              } break;
              case SELECTION_ID_SELECTION2: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_selection2.object(), 1);
              } break;
              default:
                BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
            }
      } break;
    }
    return stream;
}

inline
int MyChoiceWithDefaultValues::selectionId() const
{
    return d_selectionId;
}

template <class ACCESSOR>
int MyChoiceWithDefaultValues::accessSelection(ACCESSOR& accessor) const
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION0:
        return accessor(d_selection0.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION0]);
      case SELECTION_ID_SELECTION1:
        return accessor(d_selection1.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1]);
      case SELECTION_ID_SELECTION2:
        return accessor(d_selection2.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2]);
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
const int& MyChoiceWithDefaultValues::selection0() const
{
    BSLS_ASSERT(SELECTION_ID_SELECTION0 == d_selectionId);
    return d_selection0.object();
}

inline
const bsl::string& MyChoiceWithDefaultValues::selection1() const
{
    BSLS_ASSERT(SELECTION_ID_SELECTION1 == d_selectionId);
    return d_selection1.object();
}

inline
const MyEnumeration::Value& MyChoiceWithDefaultValues::selection2() const
{
    BSLS_ASSERT(SELECTION_ID_SELECTION2 == d_selectionId);
    return d_selection2.object();
}

inline
bool MyChoiceWithDefaultValues::isSelection0Value() const
{
    return SELECTION_ID_SELECTION0 == d_selectionId;
}

inline
bool MyChoiceWithDefaultValues::isSelection1Value() const
{
    return SELECTION_ID_SELECTION1 == d_selectionId;
}

inline
bool MyChoiceWithDefaultValues::isSelection2Value() const
{
    return SELECTION_ID_SELECTION2 == d_selectionId;
}

inline
bool MyChoiceWithDefaultValues::isUndefinedValue() const
{
    return SELECTION_ID_UNDEFINED == d_selectionId;
}


                    // -----------------------------------
                    // class MySequenceWithAnonymousChoice
                    // -----------------------------------

// CLASS METHODS
inline
int MySequenceWithAnonymousChoice::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequenceWithAnonymousChoice::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute1, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_choice, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute2, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequenceWithAnonymousChoice::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_choice, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int MySequenceWithAnonymousChoice::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_CHOICE: {
        return manipulator(&d_choice, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int MySequenceWithAnonymousChoice::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& MySequenceWithAnonymousChoice::attribute1()
{
    return d_attribute1;
}

inline
MySequenceWithAnonymousChoiceChoice& MySequenceWithAnonymousChoice::choice()
{
    return d_choice;
}

inline
bsl::string& MySequenceWithAnonymousChoice::attribute2()
{
    return d_attribute2;
}

// ACCESSORS
template <class STREAM>
STREAM& MySequenceWithAnonymousChoice::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_choice, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute2, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int MySequenceWithAnonymousChoice::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_choice, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int MySequenceWithAnonymousChoice::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_CHOICE: {
        return accessor(d_choice, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int MySequenceWithAnonymousChoice::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int MySequenceWithAnonymousChoice::attribute1() const
{
    return d_attribute1;
}

inline
const MySequenceWithAnonymousChoiceChoice& MySequenceWithAnonymousChoice::choice() const
{
    return d_choice;
}

inline
const bsl::string& MySequenceWithAnonymousChoice::attribute2() const
{
    return d_attribute2;
}



                     // ---------------------------------
                     // class MySequenceWithDefaultValues
                     // ---------------------------------

// CLASS METHODS
inline
int MySequenceWithDefaultValues::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// MANIPULATORS
template <class STREAM>
STREAM& MySequenceWithDefaultValues::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute0, 1);
            bslx::InStreamFunctions::bdexStreamIn(stream, d_attribute1, 1);
            MyEnumeration::bdexStreamIn(stream, d_attribute2, 1);
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int MySequenceWithDefaultValues::manipulateAttributes(MANIPULATOR& manipulator)
{
    int ret;

    ret = manipulator(&d_attribute0, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE0]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class MANIPULATOR>
int MySequenceWithDefaultValues::manipulateAttribute(MANIPULATOR& manipulator, int id)
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE0: {
        return manipulator(&d_attribute0, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE0]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return manipulator(&d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return manipulator(&d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class MANIPULATOR>
int MySequenceWithDefaultValues::manipulateAttribute(
        MANIPULATOR&  manipulator,
        const char   *name,
        int           nameLength)
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
                                         lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
        return NOT_FOUND;
    }

    return manipulateAttribute(manipulator, attributeInfo->d_id);
}

inline
int& MySequenceWithDefaultValues::attribute0()
{
    return d_attribute0;
}

inline
bsl::string& MySequenceWithDefaultValues::attribute1()
{
    return d_attribute1;
}

inline
MyEnumeration::Value& MySequenceWithDefaultValues::attribute2()
{
    return d_attribute2;
}

// ACCESSORS
template <class STREAM>
STREAM& MySequenceWithDefaultValues::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute0, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute1, 1);
        bslx::OutStreamFunctions::bdexStreamOut(stream, d_attribute2, 1);
      } break;
    }
    return stream;
}

template <class ACCESSOR>
int MySequenceWithDefaultValues::accessAttributes(ACCESSOR& accessor) const
{
    int ret;

    ret = accessor(d_attribute0, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE0]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
    if (ret) {
        return ret;
    }

    ret = accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
    if (ret) {
        return ret;
    }

    return ret;
}

template <class ACCESSOR>
int MySequenceWithDefaultValues::accessAttribute(ACCESSOR& accessor, int id) const
{
    enum { NOT_FOUND = -1 };

    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE0: {
        return accessor(d_attribute0, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE0]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE1: {
        return accessor(d_attribute1, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1]);
      } break;
      case ATTRIBUTE_ID_ATTRIBUTE2: {
        return accessor(d_attribute2, ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2]);
      } break;
      default:
        return NOT_FOUND;
    }
}

template <class ACCESSOR>
int MySequenceWithDefaultValues::accessAttribute(
        ACCESSOR&   accessor,
        const char *name,
        int         nameLength) const
{
    enum { NOT_FOUND = -1 };

    const bdlat_AttributeInfo *attributeInfo =
          lookupAttributeInfo(name, nameLength);
    if (0 == attributeInfo) {
       return NOT_FOUND;
    }

    return accessAttribute(accessor, attributeInfo->d_id);
}

inline
int MySequenceWithDefaultValues::attribute0() const
{
    return d_attribute0;
}

inline
const bsl::string& MySequenceWithDefaultValues::attribute1() const
{
    return d_attribute1;
}

inline
MyEnumeration::Value MySequenceWithDefaultValues::attribute2() const
{
    return d_attribute2;
}



                            // -------------------
                            // class TimingRequest
                            // -------------------

// CLASS METHODS
inline
int TimingRequest::maxSupportedBdexVersion()
{
    return 1;  // versions start at 1.
}

// CREATORS
inline
TimingRequest::TimingRequest(bslma::Allocator *basicAllocator)
: d_selectionId(SELECTION_ID_UNDEFINED)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
}

inline
TimingRequest::~TimingRequest()
{
    reset();
}

// MANIPULATORS
template <class STREAM>
STREAM& TimingRequest::bdexStreamIn(STREAM& stream, int version)
{
    if (stream) {
        switch (version) {
          case 1: {
            short selectionId;
            stream.getInt16(selectionId);
            if (!stream) {
                return stream;
            }
            switch (selectionId) {
              case SELECTION_ID_SQRT: {
                makeSqrt();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_sqrt.object(), 1);
              } break;
              case SELECTION_ID_BASIC: {
                makeBasic();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_basic.object(), 1);
              } break;
              case SELECTION_ID_BIG: {
                makeBig();
                bslx::InStreamFunctions::bdexStreamIn(
                    stream, d_big.object(), 1);
              } break;
              case SELECTION_ID_UNDEFINED: {
                reset();
              } break;
              default:
                stream.invalidate();
            }
          } break;
          default: {
            stream.invalidate();
          }
        }
    }
    return stream;
}

template <class MANIPULATOR>
int TimingRequest::manipulateSelection(MANIPULATOR& manipulator)
{
    switch (d_selectionId) {
      case TimingRequest::SELECTION_ID_SQRT:
        return manipulator(&d_sqrt.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SQRT]);
      case TimingRequest::SELECTION_ID_BASIC:
        return manipulator(&d_basic.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_BASIC]);
      case TimingRequest::SELECTION_ID_BIG:
        return manipulator(&d_big.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_BIG]);
      default:
        BSLS_ASSERT(TimingRequest::SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
Sqrt& TimingRequest::sqrt()
{
    BSLS_ASSERT(SELECTION_ID_SQRT == d_selectionId);
    return d_sqrt.object();
}

inline
BasicRecord& TimingRequest::basic()
{
    BSLS_ASSERT(SELECTION_ID_BASIC == d_selectionId);
    return d_basic.object();
}

inline
BigRecord& TimingRequest::big()
{
    BSLS_ASSERT(SELECTION_ID_BIG == d_selectionId);
    return d_big.object();
}

// ACCESSORS
template <class STREAM>
STREAM& TimingRequest::bdexStreamOut(STREAM& stream, int version) const
{
    switch (version) {
      case 1: {
            stream.putInt16(d_selectionId);
            switch (d_selectionId) {
              case SELECTION_ID_SQRT: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_sqrt.object(), 1);
              } break;
              case SELECTION_ID_BASIC: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_basic.object(), 1);
              } break;
              case SELECTION_ID_BIG: {
                bslx::OutStreamFunctions::bdexStreamOut(
                    stream, d_big.object(), 1);
              } break;
              default:
                BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
            }
      } break;
    }
    return stream;
}

inline
int TimingRequest::selectionId() const
{
    return d_selectionId;
}

template <class ACCESSOR>
int TimingRequest::accessSelection(ACCESSOR& accessor) const
{
    switch (d_selectionId) {
      case SELECTION_ID_SQRT:
        return accessor(d_sqrt.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_SQRT]);
      case SELECTION_ID_BASIC:
        return accessor(d_basic.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_BASIC]);
      case SELECTION_ID_BIG:
        return accessor(d_big.object(),
                SELECTION_INFO_ARRAY[SELECTION_INDEX_BIG]);
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return -1;
    }
}

inline
const Sqrt& TimingRequest::sqrt() const
{
    BSLS_ASSERT(SELECTION_ID_SQRT == d_selectionId);
    return d_sqrt.object();
}

inline
const BasicRecord& TimingRequest::basic() const
{
    BSLS_ASSERT(SELECTION_ID_BASIC == d_selectionId);
    return d_basic.object();
}

inline
const BigRecord& TimingRequest::big() const
{
    BSLS_ASSERT(SELECTION_ID_BIG == d_selectionId);
    return d_big.object();
}

inline
bool TimingRequest::isSqrtValue() const
{
    return SELECTION_ID_SQRT == d_selectionId;
}

inline
bool TimingRequest::isBasicValue() const
{
    return SELECTION_ID_BASIC == d_selectionId;
}

inline
bool TimingRequest::isBigValue() const
{
    return SELECTION_ID_BIG == d_selectionId;
}

inline
bool TimingRequest::isUndefinedValue() const
{
    return SELECTION_ID_UNDEFINED == d_selectionId;
}
}  // close package namespace

// FREE FUNCTIONS

inline
bool test::operator==(
        const test::Address& lhs,
        const test::Address& rhs)
{
    return  lhs.street() == rhs.street()
         && lhs.city() == rhs.city()
         && lhs.state() == rhs.state();
}

inline
bool test::operator!=(
        const test::Address& lhs,
        const test::Address& rhs)
{
    return  lhs.street() != rhs.street()
         || lhs.city() != rhs.city()
         || lhs.state() != rhs.state();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::Address& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::BasicRecord& lhs,
        const test::BasicRecord& rhs)
{
    return  lhs.i1() == rhs.i1()
         && lhs.i2() == rhs.i2()
         && lhs.dt() == rhs.dt()
         && lhs.s() == rhs.s();
}

inline
bool test::operator!=(
        const test::BasicRecord& lhs,
        const test::BasicRecord& rhs)
{
    return  lhs.i1() != rhs.i1()
         || lhs.i2() != rhs.i2()
         || lhs.dt() != rhs.dt()
         || lhs.s() != rhs.s();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::BasicRecord& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::CustomizedString& lhs,
        const test::CustomizedString& rhs)
{
    return lhs.d_value == rhs.d_value;
}

inline
bool test::operator!=(
        const test::CustomizedString& lhs,
        const test::CustomizedString& rhs)
{
    return lhs.d_value != rhs.d_value;
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::CustomizedString& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MyChoice& lhs,
        const test::MyChoice& rhs)
{
    typedef test::MyChoice Class;
    if (lhs.selectionId() == rhs.selectionId()) {
        switch (rhs.selectionId()) {
          case Class::SELECTION_ID_SELECTION1:
            return lhs.selection1() == rhs.selection1();
          case Class::SELECTION_ID_SELECTION2:
            return lhs.selection2() == rhs.selection2();
          default:
            BSLS_ASSERT(Class::SELECTION_ID_UNDEFINED == rhs.selectionId());
            return true;
        }
    }
    else {
        return false;
   }
}

inline
bool test::operator!=(
        const test::MyChoice& lhs,
        const test::MyChoice& rhs)
{
    return !(lhs == rhs);
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MyChoice& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        test::MyEnumeration::Value rhs)
{
    return test::MyEnumeration::print(stream, rhs);
}


inline
bool test::operator==(
        const test::MySequence& lhs,
        const test::MySequence& rhs)
{
    return  lhs.attribute1() == rhs.attribute1()
         && lhs.attribute2() == rhs.attribute2();
}

inline
bool test::operator!=(
        const test::MySequence& lhs,
        const test::MySequence& rhs)
{
    return  lhs.attribute1() != rhs.attribute1()
         || lhs.attribute2() != rhs.attribute2();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequence& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MySequenceWithAnonymousChoiceChoice& lhs,
        const test::MySequenceWithAnonymousChoiceChoice& rhs)
{
    typedef test::MySequenceWithAnonymousChoiceChoice Class;
    if (lhs.selectionId() == rhs.selectionId()) {
        switch (rhs.selectionId()) {
          case Class::SELECTION_ID_MY_CHOICE1:
            return lhs.myChoice1() == rhs.myChoice1();
          case Class::SELECTION_ID_MY_CHOICE2:
            return lhs.myChoice2() == rhs.myChoice2();
          default:
            BSLS_ASSERT(Class::SELECTION_ID_UNDEFINED == rhs.selectionId());
            return true;
        }
    }
    else {
        return false;
   }
}

inline
bool test::operator!=(
        const test::MySequenceWithAnonymousChoiceChoice& lhs,
        const test::MySequenceWithAnonymousChoiceChoice& rhs)
{
    return !(lhs == rhs);
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequenceWithAnonymousChoiceChoice& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MySequenceWithArray& lhs,
        const test::MySequenceWithArray& rhs)
{
    return  lhs.attribute1() == rhs.attribute1()
         && lhs.attribute2() == rhs.attribute2();
}

inline
bool test::operator!=(
        const test::MySequenceWithArray& lhs,
        const test::MySequenceWithArray& rhs)
{
    return  lhs.attribute1() != rhs.attribute1()
         || lhs.attribute2() != rhs.attribute2();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequenceWithArray& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MySequenceWithNillable& lhs,
        const test::MySequenceWithNillable& rhs)
{
    return  lhs.attribute1() == rhs.attribute1()
         && lhs.myNillable() == rhs.myNillable()
         && lhs.attribute2() == rhs.attribute2();
}

inline
bool test::operator!=(
        const test::MySequenceWithNillable& lhs,
        const test::MySequenceWithNillable& rhs)
{
    return  lhs.attribute1() != rhs.attribute1()
         || lhs.myNillable() != rhs.myNillable()
         || lhs.attribute2() != rhs.attribute2();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequenceWithNillable& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MySequenceWithNullable& lhs,
        const test::MySequenceWithNullable& rhs)
{
    return  lhs.attribute1() == rhs.attribute1()
         && lhs.attribute2() == rhs.attribute2();
}

inline
bool test::operator!=(
        const test::MySequenceWithNullable& lhs,
        const test::MySequenceWithNullable& rhs)
{
    return  lhs.attribute1() != rhs.attribute1()
         || lhs.attribute2() != rhs.attribute2();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequenceWithNullable& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::RawData& lhs,
        const test::RawData& rhs)
{
    return  lhs.charvec() == rhs.charvec()
         && lhs.ucharvec() == rhs.ucharvec();
}

inline
bool test::operator!=(
        const test::RawData& lhs,
        const test::RawData& rhs)
{
    return  lhs.charvec() != rhs.charvec()
         || lhs.ucharvec() != rhs.ucharvec();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::RawData& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::RawDataSwitched& lhs,
        const test::RawDataSwitched& rhs)
{
    return  lhs.charvec() == rhs.charvec()
         && lhs.ucharvec() == rhs.ucharvec();
}

inline
bool test::operator!=(
        const test::RawDataSwitched& lhs,
        const test::RawDataSwitched& rhs)
{
    return  lhs.charvec() != rhs.charvec()
         || lhs.ucharvec() != rhs.ucharvec();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::RawDataSwitched& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::RawDataUnformatted& lhs,
        const test::RawDataUnformatted& rhs)
{
    return  lhs.charvec() == rhs.charvec()
         && lhs.ucharvec() == rhs.ucharvec();
}

inline
bool test::operator!=(
        const test::RawDataUnformatted& lhs,
        const test::RawDataUnformatted& rhs)
{
    return  lhs.charvec() != rhs.charvec()
         || lhs.ucharvec() != rhs.ucharvec();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::RawDataUnformatted& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::Sqrt& lhs,
        const test::Sqrt& rhs)
{
    return  lhs.value() == rhs.value();
}

inline
bool test::operator!=(
        const test::Sqrt& lhs,
        const test::Sqrt& rhs)
{
    return  lhs.value() != rhs.value();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::Sqrt& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::BigRecord& lhs,
        const test::BigRecord& rhs)
{
    return  lhs.name() == rhs.name()
         && lhs.array() == rhs.array();
}

inline
bool test::operator!=(
        const test::BigRecord& lhs,
        const test::BigRecord& rhs)
{
    return  lhs.name() != rhs.name()
         || lhs.array() != rhs.array();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::BigRecord& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::Employee& lhs,
        const test::Employee& rhs)
{
    return  lhs.name() == rhs.name()
         && lhs.homeAddress() == rhs.homeAddress()
         && lhs.age() == rhs.age();
}

inline
bool test::operator!=(
        const test::Employee& lhs,
        const test::Employee& rhs)
{
    return  lhs.name() != rhs.name()
         || lhs.homeAddress() != rhs.homeAddress()
         || lhs.age() != rhs.age();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::Employee& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MyChoiceWithDefaultValues& lhs,
        const test::MyChoiceWithDefaultValues& rhs)
{
    typedef test::MyChoiceWithDefaultValues Class;
    if (lhs.selectionId() == rhs.selectionId()) {
        switch (rhs.selectionId()) {
          case Class::SELECTION_ID_SELECTION0:
            return lhs.selection0() == rhs.selection0();
          case Class::SELECTION_ID_SELECTION1:
            return lhs.selection1() == rhs.selection1();
          case Class::SELECTION_ID_SELECTION2:
            return lhs.selection2() == rhs.selection2();
          default:
            BSLS_ASSERT(Class::SELECTION_ID_UNDEFINED == rhs.selectionId());
            return true;
        }
    }
    else {
        return false;
   }
}

inline
bool test::operator!=(
        const test::MyChoiceWithDefaultValues& lhs,
        const test::MyChoiceWithDefaultValues& rhs)
{
    return !(lhs == rhs);
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MyChoiceWithDefaultValues& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MySequenceWithAnonymousChoice& lhs,
        const test::MySequenceWithAnonymousChoice& rhs)
{
    return  lhs.attribute1() == rhs.attribute1()
         && lhs.choice() == rhs.choice()
         && lhs.attribute2() == rhs.attribute2();
}

inline
bool test::operator!=(
        const test::MySequenceWithAnonymousChoice& lhs,
        const test::MySequenceWithAnonymousChoice& rhs)
{
    return  lhs.attribute1() != rhs.attribute1()
         || lhs.choice() != rhs.choice()
         || lhs.attribute2() != rhs.attribute2();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequenceWithAnonymousChoice& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::MySequenceWithDefaultValues& lhs,
        const test::MySequenceWithDefaultValues& rhs)
{
    return  lhs.attribute0() == rhs.attribute0()
         && lhs.attribute1() == rhs.attribute1()
         && lhs.attribute2() == rhs.attribute2();
}

inline
bool test::operator!=(
        const test::MySequenceWithDefaultValues& lhs,
        const test::MySequenceWithDefaultValues& rhs)
{
    return  lhs.attribute0() != rhs.attribute0()
         || lhs.attribute1() != rhs.attribute1()
         || lhs.attribute2() != rhs.attribute2();
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::MySequenceWithDefaultValues& rhs)
{
    return rhs.print(stream, 0, -1);
}


inline
bool test::operator==(
        const test::TimingRequest& lhs,
        const test::TimingRequest& rhs)
{
    typedef test::TimingRequest Class;
    if (lhs.selectionId() == rhs.selectionId()) {
        switch (rhs.selectionId()) {
          case Class::SELECTION_ID_SQRT:
            return lhs.sqrt() == rhs.sqrt();
          case Class::SELECTION_ID_BASIC:
            return lhs.basic() == rhs.basic();
          case Class::SELECTION_ID_BIG:
            return lhs.big() == rhs.big();
          default:
            BSLS_ASSERT(Class::SELECTION_ID_UNDEFINED == rhs.selectionId());
            return true;
        }
    }
    else {
        return false;
   }
}

inline
bool test::operator!=(
        const test::TimingRequest& lhs,
        const test::TimingRequest& rhs)
{
    return !(lhs == rhs);
}

inline
bsl::ostream& test::operator<<(
        bsl::ostream& stream,
        const test::TimingRequest& rhs)
{
    return rhs.print(stream, 0, -1);
}

}  // close enterprise namespace
#endif

// GENERATED BY BLP_BAS_CODEGEN_VERSION
// USING bas_codegen.pl -m msg --noTimestamps --noAggregateConversion --package test test_schema.xsd
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2019
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ------------------------------ END-OF-FILE ---------------------------------
// test_schema.cpp              *DO NOT EDIT*              @generated -*-C++-*-

#include <bdlat_formattingmode.h>
#include <bdlat_valuetypefunctions.h>
#include <bdlde_utf8util.h>
#include <bdlb_print.h>
#include <bdlb_printmethods.h>
#include <bdlb_string.h>

#include <bdlb_nullablevalue.h>
#include <bdlt_datetimetz.h>
#include <bsl_string.h>
#include <bsl_vector.h>
#include <bdlb_print.h>
#include <bslim_printer.h>
#include <bsls_assert.h>

#include <bsl_iomanip.h>
#include <bsl_limits.h>
#include <bsl_ostream.h>

namespace BloombergLP {
namespace test {

                               // -------------
                               // class Address
                               // -------------

// CONSTANTS

const char Address::CLASS_NAME[] = "Address";

const bdlat_AttributeInfo Address::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_STREET,
        "street",
        sizeof("street") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_CITY,
        "city",
        sizeof("city") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_STATE,
        "state",
        sizeof("state") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *Address::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    Address::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *Address::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_STREET:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STREET];
      case ATTRIBUTE_ID_CITY:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CITY];
      case ATTRIBUTE_ID_STATE:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_STATE];
      default:
        return 0;
    }
}

// CREATORS

Address::Address(bslma::Allocator *basicAllocator)
: d_street(basicAllocator)
, d_city(basicAllocator)
, d_state(basicAllocator)
{
}

Address::Address(const Address& original,
                 bslma::Allocator *basicAllocator)
: d_street(original.d_street, basicAllocator)
, d_city(original.d_city, basicAllocator)
, d_state(original.d_state, basicAllocator)
{
}

Address::~Address()
{
}

// MANIPULATORS

Address&
Address::operator=(const Address& rhs)
{
    if (this != &rhs) {
        d_street = rhs.d_street;
        d_city = rhs.d_city;
        d_state = rhs.d_state;
    }

    return *this;
}

void Address::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_street);
    bdlat_ValueTypeFunctions::reset(&d_city);
    bdlat_ValueTypeFunctions::reset(&d_state);
}

// ACCESSORS

bsl::ostream& Address::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("street", d_street);
    printer.printAttribute("city", d_city);
    printer.printAttribute("state", d_state);
    printer.end();
    return stream;
}



                             // -----------------
                             // class BasicRecord
                             // -----------------

// CONSTANTS

const char BasicRecord::CLASS_NAME[] = "BasicRecord";

const bdlat_AttributeInfo BasicRecord::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_I1,
        "i1",
        sizeof("i1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_I2,
        "i2",
        sizeof("i2") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_DT,
        "dt",
        sizeof("dt") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    },
    {
        ATTRIBUTE_ID_S,
        "s",
        sizeof("s") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *BasicRecord::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 4; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    BasicRecord::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *BasicRecord::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_I1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I1];
      case ATTRIBUTE_ID_I2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_I2];
      case ATTRIBUTE_ID_DT:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_DT];
      case ATTRIBUTE_ID_S:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_S];
      default:
        return 0;
    }
}

// CREATORS

BasicRecord::BasicRecord(bslma::Allocator *basicAllocator)
: d_s(basicAllocator)
, d_dt()
, d_i1()
, d_i2()
{
}

BasicRecord::BasicRecord(const BasicRecord& original,
                         bslma::Allocator *basicAllocator)
: d_s(original.d_s, basicAllocator)
, d_dt(original.d_dt)
, d_i1(original.d_i1)
, d_i2(original.d_i2)
{
}

BasicRecord::~BasicRecord()
{
}

// MANIPULATORS

BasicRecord&
BasicRecord::operator=(const BasicRecord& rhs)
{
    if (this != &rhs) {
        d_i1 = rhs.d_i1;
        d_i2 = rhs.d_i2;
        d_dt = rhs.d_dt;
        d_s = rhs.d_s;
    }

    return *this;
}

void BasicRecord::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_i1);
    bdlat_ValueTypeFunctions::reset(&d_i2);
    bdlat_ValueTypeFunctions::reset(&d_dt);
    bdlat_ValueTypeFunctions::reset(&d_s);
}

// ACCESSORS

bsl::ostream& BasicRecord::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("i1", d_i1);
    printer.printAttribute("i2", d_i2);
    printer.printAttribute("dt", d_dt);
    printer.printAttribute("s", d_s);
    printer.end();
    return stream;
}



                           // ----------------------
                           // class CustomizedString
                           // ----------------------

// PUBLIC CLASS METHODS

int CustomizedString::checkRestrictions(const bsl::string& value)
{
    if (25 < bdlde::Utf8Util::numCharacters(value.c_str(), value.length())) {
        return -1;
    }

    return 0;
}

// CONSTANTS

const char CustomizedString::CLASS_NAME[] = "CustomizedString";



                               // --------------
                               // class MyChoice
                               // --------------

// CONSTANTS

const char MyChoice::CLASS_NAME[] = "MyChoice";

const bdlat_SelectionInfo MyChoice::SELECTION_INFO_ARRAY[] = {
    {
        SELECTION_ID_SELECTION1,
        "selection1",
        sizeof("selection1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        SELECTION_ID_SELECTION2,
        "selection2",
        sizeof("selection2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_SelectionInfo *MyChoice::lookupSelectionInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_SelectionInfo& selectionInfo =
                    MyChoice::SELECTION_INFO_ARRAY[i];

        if (nameLength == selectionInfo.d_nameLength
        &&  0 == bsl::memcmp(selectionInfo.d_name_p, name, nameLength))
        {
            return &selectionInfo;
        }
    }

    return 0;
}

const bdlat_SelectionInfo *MyChoice::lookupSelectionInfo(int id)
{
    switch (id) {
      case SELECTION_ID_SELECTION1:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1];
      case SELECTION_ID_SELECTION2:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2];
      default:
        return 0;
    }
}

// CREATORS

MyChoice::MyChoice(
    const MyChoice& original,
    bslma::Allocator *basicAllocator)
: d_selectionId(original.d_selectionId)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION1: {
        new (d_selection1.buffer())
            int(original.d_selection1.object());
      } break;
      case SELECTION_ID_SELECTION2: {
        new (d_selection2.buffer())
            bsl::string(
                original.d_selection2.object(), d_allocator_p);
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }
}

// MANIPULATORS

MyChoice&
MyChoice::operator=(const MyChoice& rhs)
{
    if (this != &rhs) {
        switch (rhs.d_selectionId) {
          case SELECTION_ID_SELECTION1: {
            makeSelection1(rhs.d_selection1.object());
          } break;
          case SELECTION_ID_SELECTION2: {
            makeSelection2(rhs.d_selection2.object());
          } break;
          default:
            BSLS_ASSERT(SELECTION_ID_UNDEFINED == rhs.d_selectionId);
            reset();
        }
    }

    return *this;
}

void MyChoice::reset()
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION1: {
        // no destruction required
      } break;
      case SELECTION_ID_SELECTION2: {
        typedef bsl::string Type;
        d_selection2.object().~Type();
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }

    d_selectionId = SELECTION_ID_UNDEFINED;
}

int MyChoice::makeSelection(int selectionId)
{
    switch (selectionId) {
      case SELECTION_ID_SELECTION1: {
        makeSelection1();
      } break;
      case SELECTION_ID_SELECTION2: {
        makeSelection2();
      } break;
      case SELECTION_ID_UNDEFINED: {
        reset();
      } break;
      default:
        return -1;
    }
    return 0;
}

int MyChoice::makeSelection(const char *name, int nameLength)
{
    const bdlat_SelectionInfo *selectionInfo =
                                         lookupSelectionInfo(name, nameLength);
    if (0 == selectionInfo) {
       return -1;
    }

    return makeSelection(selectionInfo->d_id);
}

int& MyChoice::makeSelection1()
{
    if (SELECTION_ID_SELECTION1 == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_selection1.object());
    }
    else {
        reset();
        new (d_selection1.buffer())
            int();
        d_selectionId = SELECTION_ID_SELECTION1;
    }

    return d_selection1.object();
}

int& MyChoice::makeSelection1(int value)
{
    if (SELECTION_ID_SELECTION1 == d_selectionId) {
        d_selection1.object() = value;
    }
    else {
        reset();
        new (d_selection1.buffer())
                int(value);
        d_selectionId = SELECTION_ID_SELECTION1;
    }

    return d_selection1.object();
}

bsl::string& MyChoice::makeSelection2()
{
    if (SELECTION_ID_SELECTION2 == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_selection2.object());
    }
    else {
        reset();
        new (d_selection2.buffer())
                bsl::string(d_allocator_p);
        d_selectionId = SELECTION_ID_SELECTION2;
    }

    return d_selection2.object();
}

bsl::string& MyChoice::makeSelection2(const bsl::string& value)
{
    if (SELECTION_ID_SELECTION2 == d_selectionId) {
        d_selection2.object() = value;
    }
    else {
        reset();
        new (d_selection2.buffer())
                bsl::string(value, d_allocator_p);
        d_selectionId = SELECTION_ID_SELECTION2;
    }

    return d_selection2.object();
}

// ACCESSORS

bsl::ostream& MyChoice::print(
    bsl::ostream& stream,
    int           level,
    int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION1: {
        printer.printAttribute("selection1", d_selection1.object());
      }  break;
      case SELECTION_ID_SELECTION2: {
        printer.printAttribute("selection2", d_selection2.object());
      }  break;
      default:
        stream << "SELECTION UNDEFINED\n";
    }
    printer.end();
    return stream;
}


const char *MyChoice::selectionName() const
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION1:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1].name();
      case SELECTION_ID_SELECTION2:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2].name();
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return "(* UNDEFINED *)";
    }
}

                            // -------------------
                            // class MyEnumeration
                            // -------------------

// CONSTANTS

const char MyEnumeration::CLASS_NAME[] = "MyEnumeration";

const bdlat_EnumeratorInfo MyEnumeration::ENUMERATOR_INFO_ARRAY[] = {
    {
        MyEnumeration::VALUE1,
        "VALUE1",
        sizeof("VALUE1") - 1,
        ""
    },
    {
        MyEnumeration::VALUE2,
        "VALUE2",
        sizeof("VALUE2") - 1,
        ""
    }
};

// CLASS METHODS

int MyEnumeration::fromInt(MyEnumeration::Value *result, int number)
{
    switch (number) {
      case MyEnumeration::VALUE1:
      case MyEnumeration::VALUE2:
        *result = (MyEnumeration::Value)number;
        return 0;
      default:
        return -1;
    }
}

int MyEnumeration::fromString(
        MyEnumeration::Value *result,
        const char         *string,
        int                 stringLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_EnumeratorInfo& enumeratorInfo =
                    MyEnumeration::ENUMERATOR_INFO_ARRAY[i];

        if (stringLength == enumeratorInfo.d_nameLength
        &&  0 == bsl::memcmp(enumeratorInfo.d_name_p, string, stringLength))
        {
            *result = (MyEnumeration::Value)enumeratorInfo.d_value;
            return 0;
        }
    }

    return -1;
}

const char *MyEnumeration::toString(MyEnumeration::Value value)
{
    switch (value) {
      case VALUE1: {
        return "VALUE1";
      } break;
      case VALUE2: {
        return "VALUE2";
      } break;
    }

    BSLS_ASSERT(!"invalid enumerator");
    return 0;
}


                              // ----------------
                              // class MySequence
                              // ----------------

// CONSTANTS

const char MySequence::CLASS_NAME[] = "MySequence";

const bdlat_AttributeInfo MySequence::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequence::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequence::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequence::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      default:
        return 0;
    }
}

// CREATORS

MySequence::MySequence(bslma::Allocator *basicAllocator)
: d_attribute2(basicAllocator)
, d_attribute1()
{
}

MySequence::MySequence(const MySequence& original,
                       bslma::Allocator *basicAllocator)
: d_attribute2(original.d_attribute2, basicAllocator)
, d_attribute1(original.d_attribute1)
{
}

MySequence::~MySequence()
{
}

// MANIPULATORS

MySequence&
MySequence::operator=(const MySequence& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
        d_attribute2 = rhs.d_attribute2;
    }

    return *this;
}

void MySequence::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
    bdlat_ValueTypeFunctions::reset(&d_attribute2);
}

// ACCESSORS

bsl::ostream& MySequence::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", d_attribute1);
    printer.printAttribute("attribute2", d_attribute2);
    printer.end();
    return stream;
}



                 // -----------------------------------------
                 // class MySequenceWithAnonymousChoiceChoice
                 // -----------------------------------------

// CONSTANTS

const char MySequenceWithAnonymousChoiceChoice::CLASS_NAME[] = "MySequenceWithAnonymousChoiceChoice";

const bdlat_SelectionInfo MySequenceWithAnonymousChoiceChoice::SELECTION_INFO_ARRAY[] = {
    {
        SELECTION_ID_MY_CHOICE1,
        "myChoice1",
        sizeof("myChoice1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        SELECTION_ID_MY_CHOICE2,
        "myChoice2",
        sizeof("myChoice2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_SelectionInfo *MySequenceWithAnonymousChoiceChoice::lookupSelectionInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_SelectionInfo& selectionInfo =
                    MySequenceWithAnonymousChoiceChoice::SELECTION_INFO_ARRAY[i];

        if (nameLength == selectionInfo.d_nameLength
        &&  0 == bsl::memcmp(selectionInfo.d_name_p, name, nameLength))
        {
            return &selectionInfo;
        }
    }

    return 0;
}

const bdlat_SelectionInfo *MySequenceWithAnonymousChoiceChoice::lookupSelectionInfo(int id)
{
    switch (id) {
      case SELECTION_ID_MY_CHOICE1:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE1];
      case SELECTION_ID_MY_CHOICE2:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE2];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithAnonymousChoiceChoice::MySequenceWithAnonymousChoiceChoice(
    const MySequenceWithAnonymousChoiceChoice& original,
    bslma::Allocator *basicAllocator)
: d_selectionId(original.d_selectionId)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    switch (d_selectionId) {
      case SELECTION_ID_MY_CHOICE1: {
        new (d_myChoice1.buffer())
            int(original.d_myChoice1.object());
      } break;
      case SELECTION_ID_MY_CHOICE2: {
        new (d_myChoice2.buffer())
            bsl::string(
                original.d_myChoice2.object(), d_allocator_p);
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }
}

// MANIPULATORS

MySequenceWithAnonymousChoiceChoice&
MySequenceWithAnonymousChoiceChoice::operator=(const MySequenceWithAnonymousChoiceChoice& rhs)
{
    if (this != &rhs) {
        switch (rhs.d_selectionId) {
          case SELECTION_ID_MY_CHOICE1: {
            makeMyChoice1(rhs.d_myChoice1.object());
          } break;
          case SELECTION_ID_MY_CHOICE2: {
            makeMyChoice2(rhs.d_myChoice2.object());
          } break;
          default:
            BSLS_ASSERT(SELECTION_ID_UNDEFINED == rhs.d_selectionId);
            reset();
        }
    }

    return *this;
}

void MySequenceWithAnonymousChoiceChoice::reset()
{
    switch (d_selectionId) {
      case SELECTION_ID_MY_CHOICE1: {
        // no destruction required
      } break;
      case SELECTION_ID_MY_CHOICE2: {
        typedef bsl::string Type;
        d_myChoice2.object().~Type();
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }

    d_selectionId = SELECTION_ID_UNDEFINED;
}

int MySequenceWithAnonymousChoiceChoice::makeSelection(int selectionId)
{
    switch (selectionId) {
      case SELECTION_ID_MY_CHOICE1: {
        makeMyChoice1();
      } break;
      case SELECTION_ID_MY_CHOICE2: {
        makeMyChoice2();
      } break;
      case SELECTION_ID_UNDEFINED: {
        reset();
      } break;
      default:
        return -1;
    }
    return 0;
}

int MySequenceWithAnonymousChoiceChoice::makeSelection(const char *name, int nameLength)
{
    const bdlat_SelectionInfo *selectionInfo =
                                         lookupSelectionInfo(name, nameLength);
    if (0 == selectionInfo) {
       return -1;
    }

    return makeSelection(selectionInfo->d_id);
}

int& MySequenceWithAnonymousChoiceChoice::makeMyChoice1()
{
    if (SELECTION_ID_MY_CHOICE1 == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_myChoice1.object());
    }
    else {
        reset();
        new (d_myChoice1.buffer())
            int();
        d_selectionId = SELECTION_ID_MY_CHOICE1;
    }

    return d_myChoice1.object();
}

int& MySequenceWithAnonymousChoiceChoice::makeMyChoice1(int value)
{
    if (SELECTION_ID_MY_CHOICE1 == d_selectionId) {
        d_myChoice1.object() = value;
    }
    else {
        reset();
        new (d_myChoice1.buffer())
                int(value);
        d_selectionId = SELECTION_ID_MY_CHOICE1;
    }

    return d_myChoice1.object();
}

bsl::string& MySequenceWithAnonymousChoiceChoice::makeMyChoice2()
{
    if (SELECTION_ID_MY_CHOICE2 == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_myChoice2.object());
    }
    else {
        reset();
        new (d_myChoice2.buffer())
                bsl::string(d_allocator_p);
        d_selectionId = SELECTION_ID_MY_CHOICE2;
    }

    return d_myChoice2.object();
}

bsl::string& MySequenceWithAnonymousChoiceChoice::makeMyChoice2(const bsl::string& value)
{
    if (SELECTION_ID_MY_CHOICE2 == d_selectionId) {
        d_myChoice2.object() = value;
    }
    else {
        reset();
        new (d_myChoice2.buffer())
                bsl::string(value, d_allocator_p);
        d_selectionId = SELECTION_ID_MY_CHOICE2;
    }

    return d_myChoice2.object();
}

// ACCESSORS

bsl::ostream& MySequenceWithAnonymousChoiceChoice::print(
    bsl::ostream& stream,
    int           level,
    int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    switch (d_selectionId) {
      case SELECTION_ID_MY_CHOICE1: {
        printer.printAttribute("myChoice1", d_myChoice1.object());
      }  break;
      case SELECTION_ID_MY_CHOICE2: {
        printer.printAttribute("myChoice2", d_myChoice2.object());
      }  break;
      default:
        stream << "SELECTION UNDEFINED\n";
    }
    printer.end();
    return stream;
}


const char *MySequenceWithAnonymousChoiceChoice::selectionName() const
{
    switch (d_selectionId) {
      case SELECTION_ID_MY_CHOICE1:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE1].name();
      case SELECTION_ID_MY_CHOICE2:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_MY_CHOICE2].name();
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return "(* UNDEFINED *)";
    }
}

                         // -------------------------
                         // class MySequenceWithArray
                         // -------------------------

// CONSTANTS

const char MySequenceWithArray::CLASS_NAME[] = "MySequenceWithArray";

const bdlat_AttributeInfo MySequenceWithArray::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequenceWithArray::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequenceWithArray::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequenceWithArray::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithArray::MySequenceWithArray(bslma::Allocator *basicAllocator)
: d_attribute2(basicAllocator)
, d_attribute1()
{
}

MySequenceWithArray::MySequenceWithArray(const MySequenceWithArray& original,
                                         bslma::Allocator *basicAllocator)
: d_attribute2(original.d_attribute2, basicAllocator)
, d_attribute1(original.d_attribute1)
{
}

MySequenceWithArray::~MySequenceWithArray()
{
}

// MANIPULATORS

MySequenceWithArray&
MySequenceWithArray::operator=(const MySequenceWithArray& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
        d_attribute2 = rhs.d_attribute2;
    }

    return *this;
}

void MySequenceWithArray::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
    bdlat_ValueTypeFunctions::reset(&d_attribute2);
}

// ACCESSORS

bsl::ostream& MySequenceWithArray::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", d_attribute1);
    printer.printAttribute("attribute2", d_attribute2);
    printer.end();
    return stream;
}



                        // ----------------------------
                        // class MySequenceWithNillable
                        // ----------------------------

// CONSTANTS

const char MySequenceWithNillable::CLASS_NAME[] = "MySequenceWithNillable";

const bdlat_AttributeInfo MySequenceWithNillable::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_MY_NILLABLE,
        "myNillable",
        sizeof("myNillable") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
      | bdlat_FormattingMode::e_NILLABLE
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequenceWithNillable::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequenceWithNillable::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequenceWithNillable::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_MY_NILLABLE:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_MY_NILLABLE];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithNillable::MySequenceWithNillable(bslma::Allocator *basicAllocator)
: d_attribute2(basicAllocator)
, d_myNillable(basicAllocator)
, d_attribute1()
{
}

MySequenceWithNillable::MySequenceWithNillable(const MySequenceWithNillable& original,
                                               bslma::Allocator *basicAllocator)
: d_attribute2(original.d_attribute2, basicAllocator)
, d_myNillable(original.d_myNillable, basicAllocator)
, d_attribute1(original.d_attribute1)
{
}

MySequenceWithNillable::~MySequenceWithNillable()
{
}

// MANIPULATORS

MySequenceWithNillable&
MySequenceWithNillable::operator=(const MySequenceWithNillable& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
        d_myNillable = rhs.d_myNillable;
        d_attribute2 = rhs.d_attribute2;
    }

    return *this;
}

void MySequenceWithNillable::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
    bdlat_ValueTypeFunctions::reset(&d_myNillable);
    bdlat_ValueTypeFunctions::reset(&d_attribute2);
}

// ACCESSORS

bsl::ostream& MySequenceWithNillable::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", d_attribute1);
    printer.printAttribute("myNillable", d_myNillable);
    printer.printAttribute("attribute2", d_attribute2);
    printer.end();
    return stream;
}



                        // ----------------------------
                        // class MySequenceWithNullable
                        // ----------------------------

// CONSTANTS

const char MySequenceWithNullable::CLASS_NAME[] = "MySequenceWithNullable";

const bdlat_AttributeInfo MySequenceWithNullable::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequenceWithNullable::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequenceWithNullable::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequenceWithNullable::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithNullable::MySequenceWithNullable(bslma::Allocator *basicAllocator)
: d_attribute2(basicAllocator)
, d_attribute1()
{
}

MySequenceWithNullable::MySequenceWithNullable(const MySequenceWithNullable& original,
                                               bslma::Allocator *basicAllocator)
: d_attribute2(original.d_attribute2, basicAllocator)
, d_attribute1(original.d_attribute1)
{
}

MySequenceWithNullable::~MySequenceWithNullable()
{
}

// MANIPULATORS

MySequenceWithNullable&
MySequenceWithNullable::operator=(const MySequenceWithNullable& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
        d_attribute2 = rhs.d_attribute2;
    }

    return *this;
}

void MySequenceWithNullable::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
    bdlat_ValueTypeFunctions::reset(&d_attribute2);
}

// ACCESSORS

bsl::ostream& MySequenceWithNullable::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", d_attribute1);
    printer.printAttribute("attribute2", d_attribute2);
    printer.end();
    return stream;
}



                               // -------------
                               // class RawData
                               // -------------

// CONSTANTS

const char RawData::CLASS_NAME[] = "RawData";

const bdlat_AttributeInfo RawData::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_CHARVEC,
        "charvec",
        sizeof("charvec") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    },
    {
        ATTRIBUTE_ID_UCHARVEC,
        "ucharvec",
        sizeof("ucharvec") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *RawData::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    RawData::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *RawData::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_CHARVEC:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC];
      case ATTRIBUTE_ID_UCHARVEC:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC];
      default:
        return 0;
    }
}

// CREATORS

RawData::RawData(bslma::Allocator *basicAllocator)
: d_ucharvec(basicAllocator)
, d_charvec(basicAllocator)
{
}

RawData::RawData(const RawData& original,
                 bslma::Allocator *basicAllocator)
: d_ucharvec(original.d_ucharvec, basicAllocator)
, d_charvec(original.d_charvec, basicAllocator)
{
}

RawData::~RawData()
{
}

// MANIPULATORS

RawData&
RawData::operator=(const RawData& rhs)
{
    if (this != &rhs) {
        d_charvec = rhs.d_charvec;
        d_ucharvec = rhs.d_ucharvec;
    }

    return *this;
}

void RawData::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_charvec);
    bdlat_ValueTypeFunctions::reset(&d_ucharvec);
}

// ACCESSORS

bsl::ostream& RawData::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    {
        bool multilineFlag = (0 <= level);
        bdlb::Print::indent(stream, level + 1, spacesPerLevel);
        stream << (multilineFlag ? "" : " ");
        stream << "charvec = [ ";
        bdlb::Print::singleLineHexDump(
            stream, d_charvec.begin(), d_charvec.end());
        stream << " ]" << (multilineFlag ? "\n" : "");
    }
    printer.printAttribute("ucharvec", d_ucharvec);
    printer.end();
    return stream;
}



                           // ---------------------
                           // class RawDataSwitched
                           // ---------------------

// CONSTANTS

const char RawDataSwitched::CLASS_NAME[] = "RawDataSwitched";

const bdlat_AttributeInfo RawDataSwitched::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_CHARVEC,
        "charvec",
        sizeof("charvec") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_UCHARVEC,
        "ucharvec",
        sizeof("ucharvec") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *RawDataSwitched::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    RawDataSwitched::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *RawDataSwitched::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_CHARVEC:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC];
      case ATTRIBUTE_ID_UCHARVEC:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC];
      default:
        return 0;
    }
}

// CREATORS

RawDataSwitched::RawDataSwitched(bslma::Allocator *basicAllocator)
: d_ucharvec(basicAllocator)
, d_charvec(basicAllocator)
{
}

RawDataSwitched::RawDataSwitched(const RawDataSwitched& original,
                                 bslma::Allocator *basicAllocator)
: d_ucharvec(original.d_ucharvec, basicAllocator)
, d_charvec(original.d_charvec, basicAllocator)
{
}

RawDataSwitched::~RawDataSwitched()
{
}

// MANIPULATORS

RawDataSwitched&
RawDataSwitched::operator=(const RawDataSwitched& rhs)
{
    if (this != &rhs) {
        d_charvec = rhs.d_charvec;
        d_ucharvec = rhs.d_ucharvec;
    }

    return *this;
}

void RawDataSwitched::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_charvec);
    bdlat_ValueTypeFunctions::reset(&d_ucharvec);
}

// ACCESSORS

bsl::ostream& RawDataSwitched::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("charvec", d_charvec);
    printer.printAttribute("ucharvec", d_ucharvec);
    printer.end();
    return stream;
}



                          // ------------------------
                          // class RawDataUnformatted
                          // ------------------------

// CONSTANTS

const char RawDataUnformatted::CLASS_NAME[] = "RawDataUnformatted";

const bdlat_AttributeInfo RawDataUnformatted::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_CHARVEC,
        "charvec",
        sizeof("charvec") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_UCHARVEC,
        "ucharvec",
        sizeof("ucharvec") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *RawDataUnformatted::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    RawDataUnformatted::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *RawDataUnformatted::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_CHARVEC:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHARVEC];
      case ATTRIBUTE_ID_UCHARVEC:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_UCHARVEC];
      default:
        return 0;
    }
}

// CREATORS

RawDataUnformatted::RawDataUnformatted(bslma::Allocator *basicAllocator)
: d_ucharvec(basicAllocator)
, d_charvec(basicAllocator)
{
}

RawDataUnformatted::RawDataUnformatted(const RawDataUnformatted& original,
                                       bslma::Allocator *basicAllocator)
: d_ucharvec(original.d_ucharvec, basicAllocator)
, d_charvec(original.d_charvec, basicAllocator)
{
}

RawDataUnformatted::~RawDataUnformatted()
{
}

// MANIPULATORS

RawDataUnformatted&
RawDataUnformatted::operator=(const RawDataUnformatted& rhs)
{
    if (this != &rhs) {
        d_charvec = rhs.d_charvec;
        d_ucharvec = rhs.d_ucharvec;
    }

    return *this;
}

void RawDataUnformatted::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_charvec);
    bdlat_ValueTypeFunctions::reset(&d_ucharvec);
}

// ACCESSORS

bsl::ostream& RawDataUnformatted::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("charvec", d_charvec);
    printer.printAttribute("ucharvec", d_ucharvec);
    printer.end();
    return stream;
}



                                 // ----------
                                 // class Sqrt
                                 // ----------

// CONSTANTS

const char Sqrt::CLASS_NAME[] = "Sqrt";

const bdlat_AttributeInfo Sqrt::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_VALUE,
        "value",
        sizeof("value") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *Sqrt::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 1; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    Sqrt::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *Sqrt::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_VALUE:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_VALUE];
      default:
        return 0;
    }
}

// CREATORS

Sqrt::Sqrt()
: d_value()
{
}

Sqrt::Sqrt(const Sqrt& original)
: d_value(original.d_value)
{
}

Sqrt::~Sqrt()
{
}

// MANIPULATORS

Sqrt&
Sqrt::operator=(const Sqrt& rhs)
{
    if (this != &rhs) {
        d_value = rhs.d_value;
    }

    return *this;
}

void Sqrt::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_value);
}

// ACCESSORS

bsl::ostream& Sqrt::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("value", d_value);
    printer.end();
    return stream;
}



                              // ---------------
                              // class BigRecord
                              // ---------------

// CONSTANTS

const char BigRecord::CLASS_NAME[] = "BigRecord";

const bdlat_AttributeInfo BigRecord::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_NAME,
        "name",
        sizeof("name") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_ARRAY,
        "array",
        sizeof("array") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *BigRecord::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    BigRecord::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *BigRecord::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_NAME:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME];
      case ATTRIBUTE_ID_ARRAY:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ARRAY];
      default:
        return 0;
    }
}

// CREATORS

BigRecord::BigRecord(bslma::Allocator *basicAllocator)
: d_array(basicAllocator)
, d_name(basicAllocator)
{
}

BigRecord::BigRecord(const BigRecord& original,
                     bslma::Allocator *basicAllocator)
: d_array(original.d_array, basicAllocator)
, d_name(original.d_name, basicAllocator)
{
}

BigRecord::~BigRecord()
{
}

// MANIPULATORS

BigRecord&
BigRecord::operator=(const BigRecord& rhs)
{
    if (this != &rhs) {
        d_name = rhs.d_name;
        d_array = rhs.d_array;
    }

    return *this;
}

void BigRecord::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_name);
    bdlat_ValueTypeFunctions::reset(&d_array);
}

// ACCESSORS

bsl::ostream& BigRecord::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("name", d_name);
    printer.printAttribute("array", d_array);
    printer.end();
    return stream;
}



                               // --------------
                               // class Employee
                               // --------------

// CONSTANTS

const char Employee::CLASS_NAME[] = "Employee";

const bdlat_AttributeInfo Employee::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_NAME,
        "name",
        sizeof("name") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_HOME_ADDRESS,
        "homeAddress",
        sizeof("homeAddress") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    },
    {
        ATTRIBUTE_ID_AGE,
        "age",
        sizeof("age") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *Employee::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    Employee::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *Employee::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_NAME:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_NAME];
      case ATTRIBUTE_ID_HOME_ADDRESS:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_HOME_ADDRESS];
      case ATTRIBUTE_ID_AGE:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_AGE];
      default:
        return 0;
    }
}

// CREATORS

Employee::Employee(bslma::Allocator *basicAllocator)
: d_name(basicAllocator)
, d_homeAddress(basicAllocator)
, d_age()
{
}

Employee::Employee(const Employee& original,
                   bslma::Allocator *basicAllocator)
: d_name(original.d_name, basicAllocator)
, d_homeAddress(original.d_homeAddress, basicAllocator)
, d_age(original.d_age)
{
}

Employee::~Employee()
{
}

// MANIPULATORS

Employee&
Employee::operator=(const Employee& rhs)
{
    if (this != &rhs) {
        d_name = rhs.d_name;
        d_homeAddress = rhs.d_homeAddress;
        d_age = rhs.d_age;
    }

    return *this;
}

void Employee::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_name);
    bdlat_ValueTypeFunctions::reset(&d_homeAddress);
    bdlat_ValueTypeFunctions::reset(&d_age);
}

// ACCESSORS

bsl::ostream& Employee::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("name", d_name);
    printer.printAttribute("homeAddress", d_homeAddress);
    printer.printAttribute("age", d_age);
    printer.end();
    return stream;
}



                      // -------------------------------
                      // class MyChoiceWithDefaultValues
                      // -------------------------------

// CONSTANTS

const char MyChoiceWithDefaultValues::CLASS_NAME[] = "MyChoiceWithDefaultValues";

const int MyChoiceWithDefaultValues::DEFAULT_INITIALIZER_SELECTION0 = 100;

const char MyChoiceWithDefaultValues::DEFAULT_INITIALIZER_SELECTION1[] = "default";

const MyEnumeration::Value MyChoiceWithDefaultValues::DEFAULT_INITIALIZER_SELECTION2 = MyEnumeration::VALUE2;

const bdlat_SelectionInfo MyChoiceWithDefaultValues::SELECTION_INFO_ARRAY[] = {
    {
        SELECTION_ID_SELECTION0,
        "selection0",
        sizeof("selection0") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        SELECTION_ID_SELECTION1,
        "selection1",
        sizeof("selection1") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        SELECTION_ID_SELECTION2,
        "selection2",
        sizeof("selection2") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    }
};

// CLASS METHODS

const bdlat_SelectionInfo *MyChoiceWithDefaultValues::lookupSelectionInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_SelectionInfo& selectionInfo =
                    MyChoiceWithDefaultValues::SELECTION_INFO_ARRAY[i];

        if (nameLength == selectionInfo.d_nameLength
        &&  0 == bsl::memcmp(selectionInfo.d_name_p, name, nameLength))
        {
            return &selectionInfo;
        }
    }

    return 0;
}

const bdlat_SelectionInfo *MyChoiceWithDefaultValues::lookupSelectionInfo(int id)
{
    switch (id) {
      case SELECTION_ID_SELECTION0:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION0];
      case SELECTION_ID_SELECTION1:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1];
      case SELECTION_ID_SELECTION2:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2];
      default:
        return 0;
    }
}

// CREATORS

MyChoiceWithDefaultValues::MyChoiceWithDefaultValues(
    const MyChoiceWithDefaultValues& original,
    bslma::Allocator *basicAllocator)
: d_selectionId(original.d_selectionId)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION0: {
        new (d_selection0.buffer())
            int(original.d_selection0.object());
      } break;
      case SELECTION_ID_SELECTION1: {
        new (d_selection1.buffer())
            bsl::string(
                original.d_selection1.object(), d_allocator_p);
      } break;
      case SELECTION_ID_SELECTION2: {
        new (d_selection2.buffer())
            MyEnumeration::Value(original.d_selection2.object());
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }
}

// MANIPULATORS

MyChoiceWithDefaultValues&
MyChoiceWithDefaultValues::operator=(const MyChoiceWithDefaultValues& rhs)
{
    if (this != &rhs) {
        switch (rhs.d_selectionId) {
          case SELECTION_ID_SELECTION0: {
            makeSelection0(rhs.d_selection0.object());
          } break;
          case SELECTION_ID_SELECTION1: {
            makeSelection1(rhs.d_selection1.object());
          } break;
          case SELECTION_ID_SELECTION2: {
            makeSelection2(rhs.d_selection2.object());
          } break;
          default:
            BSLS_ASSERT(SELECTION_ID_UNDEFINED == rhs.d_selectionId);
            reset();
        }
    }

    return *this;
}

void MyChoiceWithDefaultValues::reset()
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION0: {
        // no destruction required
      } break;
      case SELECTION_ID_SELECTION1: {
        typedef bsl::string Type;
        d_selection1.object().~Type();
      } break;
      case SELECTION_ID_SELECTION2: {
        typedef MyEnumeration::Value Type;
        d_selection2.object().~Type();
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }

    d_selectionId = SELECTION_ID_UNDEFINED;
}

int MyChoiceWithDefaultValues::makeSelection(int selectionId)
{
    switch (selectionId) {
      case SELECTION_ID_SELECTION0: {
        makeSelection0();
      } break;
      case SELECTION_ID_SELECTION1: {
        makeSelection1();
      } break;
      case SELECTION_ID_SELECTION2: {
        makeSelection2();
      } break;
      case SELECTION_ID_UNDEFINED: {
        reset();
      } break;
      default:
        return -1;
    }
    return 0;
}

int MyChoiceWithDefaultValues::makeSelection(const char *name, int nameLength)
{
    const bdlat_SelectionInfo *selectionInfo =
                                         lookupSelectionInfo(name, nameLength);
    if (0 == selectionInfo) {
       return -1;
    }

    return makeSelection(selectionInfo->d_id);
}

int& MyChoiceWithDefaultValues::makeSelection0()
{
    if (SELECTION_ID_SELECTION0 == d_selectionId) {
        d_selection0.object() = DEFAULT_INITIALIZER_SELECTION0;
    }
    else {
        reset();
        new (d_selection0.buffer())
            int(DEFAULT_INITIALIZER_SELECTION0);
        d_selectionId = SELECTION_ID_SELECTION0;
    }

    return d_selection0.object();
}

int& MyChoiceWithDefaultValues::makeSelection0(int value)
{
    if (SELECTION_ID_SELECTION0 == d_selectionId) {
        d_selection0.object() = value;
    }
    else {
        reset();
        new (d_selection0.buffer())
                int(value);
        d_selectionId = SELECTION_ID_SELECTION0;
    }

    return d_selection0.object();
}

bsl::string& MyChoiceWithDefaultValues::makeSelection1()
{
    if (SELECTION_ID_SELECTION1 == d_selectionId) {
        d_selection1.object() = DEFAULT_INITIALIZER_SELECTION1;
    }
    else {
        reset();
        new (d_selection1.buffer())
            bsl::string(DEFAULT_INITIALIZER_SELECTION1, d_allocator_p);
        d_selectionId = SELECTION_ID_SELECTION1;
    }

    return d_selection1.object();
}

bsl::string& MyChoiceWithDefaultValues::makeSelection1(const bsl::string& value)
{
    if (SELECTION_ID_SELECTION1 == d_selectionId) {
        d_selection1.object() = value;
    }
    else {
        reset();
        new (d_selection1.buffer())
                bsl::string(value, d_allocator_p);
        d_selectionId = SELECTION_ID_SELECTION1;
    }

    return d_selection1.object();
}

MyEnumeration::Value& MyChoiceWithDefaultValues::makeSelection2()
{
    if (SELECTION_ID_SELECTION2 == d_selectionId) {
        d_selection2.object() = DEFAULT_INITIALIZER_SELECTION2;
    }
    else {
        reset();
        new (d_selection2.buffer())
            MyEnumeration::Value(DEFAULT_INITIALIZER_SELECTION2);
        d_selectionId = SELECTION_ID_SELECTION2;
    }

    return d_selection2.object();
}

MyEnumeration::Value& MyChoiceWithDefaultValues::makeSelection2(MyEnumeration::Value value)
{
    if (SELECTION_ID_SELECTION2 == d_selectionId) {
        d_selection2.object() = value;
    }
    else {
        reset();
        new (d_selection2.buffer())
                MyEnumeration::Value(value);
        d_selectionId = SELECTION_ID_SELECTION2;
    }

    return d_selection2.object();
}

// ACCESSORS

bsl::ostream& MyChoiceWithDefaultValues::print(
    bsl::ostream& stream,
    int           level,
    int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION0: {
        printer.printAttribute("selection0", d_selection0.object());
      }  break;
      case SELECTION_ID_SELECTION1: {
        printer.printAttribute("selection1", d_selection1.object());
      }  break;
      case SELECTION_ID_SELECTION2: {
        printer.printAttribute("selection2", d_selection2.object());
      }  break;
      default:
        stream << "SELECTION UNDEFINED\n";
    }
    printer.end();
    return stream;
}


const char *MyChoiceWithDefaultValues::selectionName() const
{
    switch (d_selectionId) {
      case SELECTION_ID_SELECTION0:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION0].name();
      case SELECTION_ID_SELECTION1:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION1].name();
      case SELECTION_ID_SELECTION2:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_SELECTION2].name();
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return "(* UNDEFINED *)";
    }
}

                    // -----------------------------------
                    // class MySequenceWithAnonymousChoice
                    // -----------------------------------

// CONSTANTS

const char MySequenceWithAnonymousChoice::CLASS_NAME[] = "MySequenceWithAnonymousChoice";

const bdlat_AttributeInfo MySequenceWithAnonymousChoice::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_CHOICE,
        "Choice",
        sizeof("Choice") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
      | bdlat_FormattingMode::e_UNTAGGED
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequenceWithAnonymousChoice::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    if (bdlb::String::areEqualCaseless("myChoice1", name, nameLength)) {
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE];
    }

    if (bdlb::String::areEqualCaseless("myChoice2", name, nameLength)) {
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE];
    }

    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequenceWithAnonymousChoice::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequenceWithAnonymousChoice::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_CHOICE:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_CHOICE];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithAnonymousChoice::MySequenceWithAnonymousChoice(bslma::Allocator *basicAllocator)
: d_attribute2(basicAllocator)
, d_choice(basicAllocator)
, d_attribute1()
{
}

MySequenceWithAnonymousChoice::MySequenceWithAnonymousChoice(const MySequenceWithAnonymousChoice& original,
                                                             bslma::Allocator *basicAllocator)
: d_attribute2(original.d_attribute2, basicAllocator)
, d_choice(original.d_choice, basicAllocator)
, d_attribute1(original.d_attribute1)
{
}

MySequenceWithAnonymousChoice::~MySequenceWithAnonymousChoice()
{
}

// MANIPULATORS

MySequenceWithAnonymousChoice&
MySequenceWithAnonymousChoice::operator=(const MySequenceWithAnonymousChoice& rhs)
{
    if (this != &rhs) {
        d_attribute1 = rhs.d_attribute1;
        d_choice = rhs.d_choice;
        d_attribute2 = rhs.d_attribute2;
    }

    return *this;
}

void MySequenceWithAnonymousChoice::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_attribute1);
    bdlat_ValueTypeFunctions::reset(&d_choice);
    bdlat_ValueTypeFunctions::reset(&d_attribute2);
}

// ACCESSORS

bsl::ostream& MySequenceWithAnonymousChoice::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute1", d_attribute1);
    printer.printAttribute("choice", d_choice);
    printer.printAttribute("attribute2", d_attribute2);
    printer.end();
    return stream;
}



                     // ---------------------------------
                     // class MySequenceWithDefaultValues
                     // ---------------------------------

// CONSTANTS

const char MySequenceWithDefaultValues::CLASS_NAME[] = "MySequenceWithDefaultValues";

const int MySequenceWithDefaultValues::DEFAULT_INITIALIZER_ATTRIBUTE0 = 100;

const char MySequenceWithDefaultValues::DEFAULT_INITIALIZER_ATTRIBUTE1[] = "default";

const MyEnumeration::Value MySequenceWithDefaultValues::DEFAULT_INITIALIZER_ATTRIBUTE2 = MyEnumeration::VALUE2;

const bdlat_AttributeInfo MySequenceWithDefaultValues::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_ATTRIBUTE0,
        "attribute0",
        sizeof("attribute0") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE1,
        "attribute1",
        sizeof("attribute1") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_ATTRIBUTE2,
        "attribute2",
        sizeof("attribute2") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *MySequenceWithDefaultValues::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    MySequenceWithDefaultValues::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *MySequenceWithDefaultValues::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_ATTRIBUTE0:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE0];
      case ATTRIBUTE_ID_ATTRIBUTE1:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE1];
      case ATTRIBUTE_ID_ATTRIBUTE2:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_ATTRIBUTE2];
      default:
        return 0;
    }
}

// CREATORS

MySequenceWithDefaultValues::MySequenceWithDefaultValues(bslma::Allocator *basicAllocator)
: d_attribute1(DEFAULT_INITIALIZER_ATTRIBUTE1, basicAllocator)
, d_attribute0(DEFAULT_INITIALIZER_ATTRIBUTE0)
, d_attribute2(DEFAULT_INITIALIZER_ATTRIBUTE2)
{
}

MySequenceWithDefaultValues::MySequenceWithDefaultValues(const MySequenceWithDefaultValues& original,
                                                         bslma::Allocator *basicAllocator)
: d_attribute1(original.d_attribute1, basicAllocator)
, d_attribute0(original.d_attribute0)
, d_attribute2(original.d_attribute2)
{
}

MySequenceWithDefaultValues::~MySequenceWithDefaultValues()
{
}

// MANIPULATORS

MySequenceWithDefaultValues&
MySequenceWithDefaultValues::operator=(const MySequenceWithDefaultValues& rhs)
{
    if (this != &rhs) {
        d_attribute0 = rhs.d_attribute0;
        d_attribute1 = rhs.d_attribute1;
        d_attribute2 = rhs.d_attribute2;
    }

    return *this;
}

void MySequenceWithDefaultValues::reset()
{
    d_attribute0 = DEFAULT_INITIALIZER_ATTRIBUTE0;
    d_attribute1 = DEFAULT_INITIALIZER_ATTRIBUTE1;
    d_attribute2 = DEFAULT_INITIALIZER_ATTRIBUTE2;
}

// ACCESSORS

bsl::ostream& MySequenceWithDefaultValues::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("attribute0", d_attribute0);
    printer.printAttribute("attribute1", d_attribute1);
    printer.printAttribute("attribute2", d_attribute2);
    printer.end();
    return stream;
}



                            // -------------------
                            // class TimingRequest
                            // -------------------

// CONSTANTS

const char TimingRequest::CLASS_NAME[] = "TimingRequest";

const bdlat_SelectionInfo TimingRequest::SELECTION_INFO_ARRAY[] = {
    {
        SELECTION_ID_SQRT,
        "sqrt",
        sizeof("sqrt") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    },
    {
        SELECTION_ID_BASIC,
        "basic",
        sizeof("basic") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    },
    {
        SELECTION_ID_BIG,
        "big",
        sizeof("big") - 1,
        "",
        bdlat_FormattingMode::e_DEFAULT
    }
};

// CLASS METHODS

const bdlat_SelectionInfo *TimingRequest::lookupSelectionInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 3; ++i) {
        const bdlat_SelectionInfo& selectionInfo =
                    TimingRequest::SELECTION_INFO_ARRAY[i];

        if (nameLength == selectionInfo.d_nameLength
        &&  0 == bsl::memcmp(selectionInfo.d_name_p, name, nameLength))
        {
            return &selectionInfo;
        }
    }

    return 0;
}

const bdlat_SelectionInfo *TimingRequest::lookupSelectionInfo(int id)
{
    switch (id) {
      case SELECTION_ID_SQRT:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_SQRT];
      case SELECTION_ID_BASIC:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_BASIC];
      case SELECTION_ID_BIG:
        return &SELECTION_INFO_ARRAY[SELECTION_INDEX_BIG];
      default:
        return 0;
    }
}

// CREATORS

TimingRequest::TimingRequest(
    const TimingRequest& original,
    bslma::Allocator *basicAllocator)
: d_selectionId(original.d_selectionId)
, d_allocator_p(bslma::Default::allocator(basicAllocator))
{
    switch (d_selectionId) {
      case SELECTION_ID_SQRT: {
        new (d_sqrt.buffer())
            Sqrt(original.d_sqrt.object());
      } break;
      case SELECTION_ID_BASIC: {
        new (d_basic.buffer())
            BasicRecord(
                original.d_basic.object(), d_allocator_p);
      } break;
      case SELECTION_ID_BIG: {
        new (d_big.buffer())
            BigRecord(
                original.d_big.object(), d_allocator_p);
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }
}

// MANIPULATORS

TimingRequest&
TimingRequest::operator=(const TimingRequest& rhs)
{
    if (this != &rhs) {
        switch (rhs.d_selectionId) {
          case SELECTION_ID_SQRT: {
            makeSqrt(rhs.d_sqrt.object());
          } break;
          case SELECTION_ID_BASIC: {
            makeBasic(rhs.d_basic.object());
          } break;
          case SELECTION_ID_BIG: {
            makeBig(rhs.d_big.object());
          } break;
          default:
            BSLS_ASSERT(SELECTION_ID_UNDEFINED == rhs.d_selectionId);
            reset();
        }
    }

    return *this;
}

void TimingRequest::reset()
{
    switch (d_selectionId) {
      case SELECTION_ID_SQRT: {
        d_sqrt.object().~Sqrt();
      } break;
      case SELECTION_ID_BASIC: {
        d_basic.object().~BasicRecord();
      } break;
      case SELECTION_ID_BIG: {
        d_big.object().~BigRecord();
      } break;
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
    }

    d_selectionId = SELECTION_ID_UNDEFINED;
}

int TimingRequest::makeSelection(int selectionId)
{
    switch (selectionId) {
      case SELECTION_ID_SQRT: {
        makeSqrt();
      } break;
      case SELECTION_ID_BASIC: {
        makeBasic();
      } break;
      case SELECTION_ID_BIG: {
        makeBig();
      } break;
      case SELECTION_ID_UNDEFINED: {
        reset();
      } break;
      default:
        return -1;
    }
    return 0;
}

int TimingRequest::makeSelection(const char *name, int nameLength)
{
    const bdlat_SelectionInfo *selectionInfo =
                                         lookupSelectionInfo(name, nameLength);
    if (0 == selectionInfo) {
       return -1;
    }

    return makeSelection(selectionInfo->d_id);
}

Sqrt& TimingRequest::makeSqrt()
{
    if (SELECTION_ID_SQRT == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_sqrt.object());
    }
    else {
        reset();
        new (d_sqrt.buffer())
            Sqrt();
        d_selectionId = SELECTION_ID_SQRT;
    }

    return d_sqrt.object();
}

Sqrt& TimingRequest::makeSqrt(const Sqrt& value)
{
    if (SELECTION_ID_SQRT == d_selectionId) {
        d_sqrt.object() = value;
    }
    else {
        reset();
        new (d_sqrt.buffer())
                Sqrt(value);
        d_selectionId = SELECTION_ID_SQRT;
    }

    return d_sqrt.object();
}

BasicRecord& TimingRequest::makeBasic()
{
    if (SELECTION_ID_BASIC == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_basic.object());
    }
    else {
        reset();
        new (d_basic.buffer())
                BasicRecord(d_allocator_p);
        d_selectionId = SELECTION_ID_BASIC;
    }

    return d_basic.object();
}

BasicRecord& TimingRequest::makeBasic(const BasicRecord& value)
{
    if (SELECTION_ID_BASIC == d_selectionId) {
        d_basic.object() = value;
    }
    else {
        reset();
        new (d_basic.buffer())
                BasicRecord(value, d_allocator_p);
        d_selectionId = SELECTION_ID_BASIC;
    }

    return d_basic.object();
}

BigRecord& TimingRequest::makeBig()
{
    if (SELECTION_ID_BIG == d_selectionId) {
        bdlat_ValueTypeFunctions::reset(&d_big.object());
    }
    else {
        reset();
        new (d_big.buffer())
                BigRecord(d_allocator_p);
        d_selectionId = SELECTION_ID_BIG;
    }

    return d_big.object();
}

BigRecord& TimingRequest::makeBig(const BigRecord& value)
{
    if (SELECTION_ID_BIG == d_selectionId) {
        d_big.object() = value;
    }
    else {
        reset();
        new (d_big.buffer())
                BigRecord(value, d_allocator_p);
        d_selectionId = SELECTION_ID_BIG;
    }

    return d_big.object();
}

// ACCESSORS

bsl::ostream& TimingRequest::print(
    bsl::ostream& stream,
    int           level,
    int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    switch (d_selectionId) {
      case SELECTION_ID_SQRT: {
        printer.printAttribute("sqrt", d_sqrt.object());
      }  break;
      case SELECTION_ID_BASIC: {
        printer.printAttribute("basic", d_basic.object());
      }  break;
      case SELECTION_ID_BIG: {
        printer.printAttribute("big", d_big.object());
      }  break;
      default:
        stream << "SELECTION UNDEFINED\n";
    }
    printer.end();
    return stream;
}


const char *TimingRequest::selectionName() const
{
    switch (d_selectionId) {
      case SELECTION_ID_SQRT:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_SQRT].name();
      case SELECTION_ID_BASIC:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_BASIC].name();
      case SELECTION_ID_BIG:
        return SELECTION_INFO_ARRAY[SELECTION_INDEX_BIG].name();
      default:
        BSLS_ASSERT(SELECTION_ID_UNDEFINED == d_selectionId);
        return "(* UNDEFINED *)";
    }
}
}  // close package namespace
}  // close enterprise namespace

// GENERATED BY BLP_BAS_CODEGEN_VERSION
// USING bas_codegen.pl -m msg --noTimestamps --noAggregateConversion --package test test_schema.xsd
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2019
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ------------------------------ END-OF-FILE ---------------------------------
// ************************* END OF GENERATED CODE ****************************

// ============================================================================
//                               USAGE EXAMPLE
// ----------------------------------------------------------------------------
//..
    namespace BloombergLP {
    namespace usage {

    struct EmployeeRecord {
        // This struct represents a sequence containing a 'string' member, an
        // 'int' member, and a 'float' member.

        // CONSTANTS
        enum {
            NAME_ATTRIBUTE_ID   = 1,
            AGE_ATTRIBUTE_ID    = 2,
            SALARY_ATTRIBUTE_ID = 3
        };

        // DATA
        bsl::string d_name;
        int         d_age;
        float       d_salary;

        // CREATORS
        EmployeeRecord();
            // Create an 'EmployeeRecord' having the attributes:
            //..
            //  d_name   == ""
            //  d_age    == 0
            //  d_salary = 0.0
            //..
        EmployeeRecord(const bsl::string& name, int age, float salary);
            // Create an 'EmployeeRecord' object having the specified
            // 'name', 'age', and 'salary' attributes.

        // ACCESSORS
        const bsl::string& name()   const;
        int                age()    const;
        float              salary() const;
    };

    // CREATORS
    EmployeeRecord::EmployeeRecord()
    : d_name()
    , d_age()
    , d_salary()
    {
    }

    EmployeeRecord::EmployeeRecord(const bsl::string& name,
                                   int               age,
                                   float             salary)
    : d_name(name)
    , d_age(age)
    , d_salary(salary)
    {
    }

    // ACCESSORS
    const bsl::string& EmployeeRecord::name() const
    {
        return d_name;
    }

    int EmployeeRecord::age() const
    {
        return d_age;
    }

    float EmployeeRecord::salary() const
    {
        return d_salary;
    }

    }  // close namespace 'usage'

    namespace usage {

    template <typename MANIPULATOR>
    int bdlat_sequenceManipulateAttribute(EmployeeRecord *object,
                                          MANIPULATOR&    manipulator,
                                          const char     *attributeName,
                                          int             attributeNameLength);
    template <typename MANIPULATOR>
    int bdlat_sequenceManipulateAttribute(EmployeeRecord *object,
                                          MANIPULATOR&    manipulator,
                                          int             attributeId);
    template <typename MANIPULATOR>
    int bdlat_sequenceManipulateAttributes(EmployeeRecord *object,
                                           MANIPULATOR&    manipulator);
    template <typename ACCESSOR>
    int bdlat_sequenceAccessAttribute(
                                   const EmployeeRecord&  object,
                                   ACCESSOR&              accessor,
                                   const char            *attributeName,
                                   int                    attributeNameLength);
    template <typename ACCESSOR>
    int bdlat_sequenceAccessAttribute(const EmployeeRecord& object,
                                      ACCESSOR&             accessor,
                                      int                   attributeId);
    template <typename ACCESSOR>
    int bdlat_sequenceAccessAttributes(const EmployeeRecord& object,
                                       ACCESSOR&             accessor);
    bool bdlat_sequenceHasAttribute(
                                   const EmployeeRecord&  object,
                                   const char            *attributeName,
                                   int                    attributeNameLength);
    bool bdlat_sequenceHasAttribute(const EmployeeRecord& object,
                                    int                   attributeId);

    }  // close namespace 'usage'

    template <typename MANIPULATOR>
    int usage::bdlat_sequenceManipulateAttribute(
                                           EmployeeRecord *object,
                                           MANIPULATOR&    manipulator,
                                           const char     *attributeName,
                                           int             attributeNameLength)
    {
        enum { k_NOT_FOUND = -1 };

        if (bdlb::String::areEqualCaseless("name",
                                           attributeName,
                                           attributeNameLength)) {

            return bdlat_sequenceManipulateAttribute(
                                            object,
                                            manipulator,
                                            EmployeeRecord::NAME_ATTRIBUTE_ID);
        }

        if (bdlb::String::areEqualCaseless("age",
                                           attributeName,
                                           attributeNameLength)) {

            return bdlat_sequenceManipulateAttribute(
                                             object,
                                             manipulator,
                                             EmployeeRecord::AGE_ATTRIBUTE_ID);
        }

        if (bdlb::String::areEqualCaseless("salary",
                                           attributeName,
                                           attributeNameLength)) {

            return bdlat_sequenceManipulateAttribute(
                                          object,
                                          manipulator,
                                          EmployeeRecord::SALARY_ATTRIBUTE_ID);
        }

        return k_NOT_FOUND;
    }

    template <typename MANIPULATOR>
    int usage::bdlat_sequenceManipulateAttribute(EmployeeRecord  *object,
                                                 MANIPULATOR&     manipulator,
                                                 int              attributeId)
    {
        enum { k_NOT_FOUND = -1 };

        switch (attributeId) {
          case EmployeeRecord::NAME_ATTRIBUTE_ID: {
            bdlat_AttributeInfo info;

            info.annotation()     = "Name of employee";
            info.formattingMode() = bdlat_FormattingMode::e_DEFAULT;
            info.id()             = EmployeeRecord::NAME_ATTRIBUTE_ID;
            info.name()           = "name";
            info.nameLength()     = 4;

            return manipulator(&object->d_name, info);
          }
          case EmployeeRecord::AGE_ATTRIBUTE_ID: {
            bdlat_AttributeInfo info;

            info.annotation()     = "Age of employee";
            info.formattingMode() = bdlat_FormattingMode::e_DEFAULT;
            info.id()             = EmployeeRecord::AGE_ATTRIBUTE_ID;
            info.name()           = "age";
            info.nameLength()     = 3;

            return manipulator(&object->d_age, info);
          }
          case EmployeeRecord::SALARY_ATTRIBUTE_ID: {
            bdlat_AttributeInfo info;

            info.annotation()     = "Salary of employee";
            info.formattingMode() = bdlat_FormattingMode::e_DEFAULT;
            info.id()             = EmployeeRecord::SALARY_ATTRIBUTE_ID;
            info.name()           = "salary";
            info.nameLength()     = 6;

            return manipulator(&object->d_salary, info);
          }
          default: {
              return k_NOT_FOUND;
          }
        }
    }

    template <typename MANIPULATOR>
    int usage::bdlat_sequenceManipulateAttributes(
                                                 EmployeeRecord   *object,
                                                 MANIPULATOR&      manipulator)
    {
        int retVal;

        retVal = bdlat_sequenceManipulateAttribute(
                                            object,
                                            manipulator,
                                            EmployeeRecord::NAME_ATTRIBUTE_ID);

        if (0 != retVal) {
            return retVal;
        }

        retVal = bdlat_sequenceManipulateAttribute(
                                             object,
                                             manipulator,
                                             EmployeeRecord::AGE_ATTRIBUTE_ID);

        if (0 != retVal) {
            return retVal;
        }

        retVal = bdlat_sequenceManipulateAttribute(
                                          object,
                                          manipulator,
                                          EmployeeRecord::SALARY_ATTRIBUTE_ID);

        return retVal;
    }

    // ACCESSORS
    template <typename ACCESSOR>
    int usage::bdlat_sequenceAccessAttribute(
                                    const EmployeeRecord&  object,
                                    ACCESSOR&              accessor,
                                    const char            *attributeName,
                                    int                    attributeNameLength)
    {
        enum { k_NOT_FOUND = -1 };

        if (bdlb::String::areEqualCaseless("name",
                                           attributeName,
                                           attributeNameLength)) {
            return bdlat_sequenceAccessAttribute(
                                            object,
                                            accessor,
                                            EmployeeRecord::NAME_ATTRIBUTE_ID);
        }

        if (bdlb::String::areEqualCaseless("age",
                                           attributeName,
                                           attributeNameLength)) {
            return bdlat_sequenceAccessAttribute(
                                             object,
                                             accessor,
                                             EmployeeRecord::AGE_ATTRIBUTE_ID);
        }

        if (bdlb::String::areEqualCaseless("salary",
                                           attributeName,
                                           attributeNameLength)) {

            return bdlat_sequenceAccessAttribute(
                                          object,
                                          accessor,
                                          EmployeeRecord::SALARY_ATTRIBUTE_ID);
        }

        return k_NOT_FOUND;
    }

    template <typename ACCESSOR>
    int usage::bdlat_sequenceAccessAttribute(const EmployeeRecord& object,
                                             ACCESSOR&             accessor,
                                             int                   attributeId)
    {
        enum { k_NOT_FOUND = -1 };

        switch (attributeId) {
          case EmployeeRecord::NAME_ATTRIBUTE_ID: {
            bdlat_AttributeInfo info;

            info.annotation()     = "Name of employee";
            info.formattingMode() = bdlat_FormattingMode::e_DEFAULT;
            info.id()             = EmployeeRecord::NAME_ATTRIBUTE_ID;
            info.name()           = "name";
            info.nameLength()     = 4;

            return accessor(object.d_name, info);
          }
          case EmployeeRecord::AGE_ATTRIBUTE_ID: {
            bdlat_AttributeInfo info;

            info.annotation()     = "Age of employee";
            info.formattingMode() = bdlat_FormattingMode::e_DEFAULT;
            info.id()             = EmployeeRecord::AGE_ATTRIBUTE_ID;
            info.name()           = "age";
            info.nameLength()     = 3;

            return accessor(object.d_age, info);
          }
          case EmployeeRecord::SALARY_ATTRIBUTE_ID: {
            bdlat_AttributeInfo info;

            info.annotation()     = "Salary of employee";
            info.formattingMode() = bdlat_FormattingMode::e_DEFAULT;
            info.id()             = EmployeeRecord::SALARY_ATTRIBUTE_ID;
            info.name()           = "salary";
            info.nameLength()     = 6;

            return accessor(object.d_salary, info);
          }
          default: {
              return k_NOT_FOUND;
          }
        }
    }

    template <typename ACCESSOR>
    int usage::bdlat_sequenceAccessAttributes(const EmployeeRecord& object,
                                              ACCESSOR&             accessor)
    {
        int retVal;

        retVal = bdlat_sequenceAccessAttribute(
                                            object,
                                            accessor,
                                            EmployeeRecord::NAME_ATTRIBUTE_ID);

        if (0 != retVal) {
            return retVal;
        }

        retVal = bdlat_sequenceAccessAttribute(
                                             object,
                                             accessor,
                                             EmployeeRecord::AGE_ATTRIBUTE_ID);

        if (0 != retVal) {
            return retVal;
        }

        retVal = bdlat_sequenceAccessAttribute(
                                          object,
                                          accessor,
                                          EmployeeRecord::SALARY_ATTRIBUTE_ID);

        return retVal;
    }

    bool usage::bdlat_sequenceHasAttribute(
                                    const EmployeeRecord&  ,
                                    const char            *attributeName,
                                    int                    attributeNameLength)
    {
        return bdlb::String::areEqualCaseless("name",
                                              attributeName,
                                              attributeNameLength)
            || bdlb::String::areEqualCaseless("age",
                                              attributeName,
                                              attributeNameLength)
            || bdlb::String::areEqualCaseless("salary",
                                              attributeName,
                                              attributeNameLength);
    }

    bool usage::bdlat_sequenceHasAttribute(const EmployeeRecord& ,
                                           int                   attributeId)
    {
        return EmployeeRecord::NAME_ATTRIBUTE_ID   == attributeId
            || EmployeeRecord::AGE_ATTRIBUTE_ID    == attributeId
            || EmployeeRecord::SALARY_ATTRIBUTE_ID == attributeId;
    }

    namespace bdlat_SequenceFunctions {

        template <>
        struct IsSequence<usage::EmployeeRecord> {
            enum { VALUE = 1 };
        };

    }  // close namespace 'bdlat_SequenceFunctions'
    }  // close enterprise namespace

static void usageExample()
{
    using namespace BloombergLP;
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: Decoding an Employee Record
/// - - - - - - - - - - - - - - - - - - -
// Suppose that an "employee record" consists of a sequence of attributes --
// 'name', 'age', and 'salary' -- that are of types 'bsl::string', 'int', and
// 'float', respectively.  Furthermore, we have a need to BER encode employee
// records as a sequence of values (for out-of-process consumption).
//
// Assume that we have defined a 'usage::EmployeeRecord' class to represent
// employee record values, and assume that we have provided the 'bdlat'
// specializations that allow the 'balber' codec components to represent class
// values as a sequence of BER primitive values.  See
// {'bdlat_sequencefunctions'|Usage} for details of creating specializations
// for a sequence type.
//
// First, we create an employee record object having typical values:
//..
    usage::EmployeeRecord bob("Bob", 56, 1234.00);
    ASSERT("Bob"   == bob.name());
    ASSERT(  56    == bob.age());
    ASSERT(1234.00 == bob.salary());
//..
// Next, we create a 'balber::Encoder' object and use it to encode our 'bob'
// object.  Here, to facilitate the examination of our results, the BER
// encoding data is delivered to a 'bslsb::MemOutStreamBuf' object:
//..
    bdlsb::MemOutStreamBuf osb;
    balber::BerEncoder     encoder;
    int                    rc = encoder.encode(&osb, bob);
    ASSERT( 0 == rc);
    ASSERT(18 == osb.length());
//..
// Now, we create a 'bdlsb::FixedMemInStreamBuf' object to manage our access
// to the data portion of the 'bdlsb::MemOutStreamBuf' (where our BER encoding
// resides), decode the values found there, and use them to set the value
// of an 'usage::EmployeeRecord' object.
//..
    balber::BerDecoderOptions  options;
    balber::BerDecoder         decoder(&options);
    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
    usage::EmployeeRecord      obj;

    rc = decoder.decode(&isb, &obj);
    ASSERT(0 == rc);
//..
// Finally, we confirm that the object defined by the BER encoding has the
// same value as the original object.
//..
    ASSERT(bob.name()   == obj.name());
    ASSERT(bob.age()    == obj.age());
    ASSERT(bob.salary() == obj.salary());
//..
}

// ============================================================================
//                               MAIN PROGRAM
// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    int        test = argc > 1 ? bsl::atoi(argv[1]) : 0;
            verbose = argc > 2;
        veryVerbose = argc > 3;
    veryVeryVerbose = argc > 4;

    bslma::TestAllocator ta("testAllocator", veryVeryVerbose);
    bslma::TestAllocatorMonitor tam(&ta);

    // CONCERN: 'BSLS_REVIEW' failures should lead to test failures.
    bsls::ReviewFailureHandlerGuard reviewGuard(&bsls::Review::failByAbort);

    balber::BerDecoderOptions  options;

    if (veryVeryVerbose) {
       options.setTraceLevel(1);
    }

    balber::BerEncoder         encoder(0);
    balber::BerDecoder         decoder(&options);

    bsl::cout << "TEST " << __FILE__ << " CASE " << test << bsl::endl;;

    switch (test) { case 0:  // Zero is always the leading case.
      case 21: {
        // --------------------------------------------------------------------
        // TESTING USAGE EXAMPLE
        //   Extracted from component header file.
        //
        // Concerns:
        //: 1 The usage example provided in the component header file compiles,
        //:   links, and runs as shown.
        //
        // Plan:
        //: 1 Incorporate usage example from header into test driver, remove
        //:   leading comment characters, and replace 'assert' with 'ASSERT'.
        //:   (C-1)
        //
        // Testing:
        //   USAGE EXAMPLE
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Usage Example"
                               << "\n=====================" << bsl::endl;

        usageExample();

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 20: {
        // --------------------------------------------------------------------
        // TESTING decoding sequences of maximum size
        //   Ensure that decoding sequences smaller than the configured maximum
        //   sequence size succeeds and decodeing sequences larger than the
        //   configured maximum sequence size fails.
        //
        // Concerns:
        //: 1 Decoding an empty 'vector<char>' with a degerate maximum decoded
        //:   sequence size of 0 successfully decodes the empty vector.
        //:
        //: 2 Decoding a single-element 'vector<char>' with a degenerate
        //:   maximum decoded sequence size of 0 fails.
        //;
        //: 3 Docoding en empty 'vector<char>' with a maximum decoded sequence
        //:   size of 1 successfully decodes the empty vector.
        //:
        //: 4 Decoding an single-element 'vector<char>' with a maximum decoded
        //:   sequence size of 1 successfully decodes the vector.
        //:
        //: 5 For various maximum decoded sequence sizes, decoding a
        //:   'vector<char>' less than or exactly equal to that size succeeds.
        //:
        //: 6 For various maximum decoded sequence sizes, decoding a
        //:   'vector<char>' of a greater size fails.
        //
        // Plan:
        //: 1 For maximum sequence sizes of 0, 1, 1 Megabyte, and 1 Gigabyte,
        //:   attmpt to decode a 'vector<char>' of 1 less than, exactly equal
        //:   to, and 1 greater than the maximum sequence size.
        //:
        //: 2 Verify that decoding vectors of sizes less than or equal to the
        //:   configured maximum sequence size succeeds.
        //:
        //: 3 Verify that decoding vectors of sizes greater than the configured
        //:   maximum sequence size fails.
        //
        // Testing:
        //   int decode(bsl::streambuf *streamBuf, TYPE *variable);
        // --------------------------------------------------------------------

        if (verbose)
            bsl::cout << "\nTesting decoding sequences of maximum size"
                      << "\n=========================================="
                      << bsl::endl;

        // First, ensure that the default maximum sequence size is 1GB.  This
        // assertion will need to be changed as the default value for the
        // maximum sequence size in 'balber::BerDecoderOptions' changes.

        balber::BerDecoderOptions options;
        ASSERT(1 * 1024 * 1024 * 1024 == options.maxSequenceSize());

        static const int DEFAULT_MAX_SIZE = -1;

        static const struct {
            int  d_lineNum;   // source line number
            int  d_maxSize;   // maximum sequence size to allow for decoding
            int  d_size;      // size of data to encode and decode
            bool d_canDecode; // flag indicating if decoding should succeed
        } DATA[] = {
            // Line Max Sequence Size        Sequence Size        Can Decode
            // ---- ----------------- --------------------------- ----------
            {    L_,                0,                          0, true     },
            {    L_,                0,                          1, false    },
            {    L_,                0,        1 * 1024 * 1024 - 1, false    },
            {    L_,                0,        1 * 1024 * 1024    , false    },
            {    L_,                0,        1 * 1024 * 1024 + 1, false    },
            {    L_,                1,                          0, true     },
            {    L_,                1,                          1, true     },
            {    L_,                1,        1 * 1024 * 1024 - 1, false    },
            {    L_,                1,        1 * 1024 * 1024    , false    },
            {    L_,                1,        1 * 1024 * 1024 + 1, false    },
            {    L_,  1 * 1024 * 1024,                          0, true     },
            {    L_,  1 * 1024 * 1024,                          1, true     },
            {    L_,  1 * 1024 * 1024,        1 * 1024 * 1024 - 1, true     },
            {    L_,  1 * 1024 * 1024,        1 * 1024 * 1024    , true     },
            {    L_,  1 * 1024 * 1024,        1 * 1024 * 1024 + 1, false    },
            {    L_,  1 * 1024 * 1024,       32 * 1024 * 1024 - 1, false    },
            {    L_,  1 * 1024 * 1024,       32 * 1024 * 1024    , false    },
            {    L_,  1 * 1024 * 1024,       32 * 1024 * 1024 + 1, false    },
            {    L_, DEFAULT_MAX_SIZE,                          0, true     },
            {    L_, DEFAULT_MAX_SIZE,                          1, true     },
            {    L_, DEFAULT_MAX_SIZE,       32 * 1024 * 1024 - 1, true     },
            {    L_, DEFAULT_MAX_SIZE,       32 * 1024 * 1024    , true     },
            {    L_, DEFAULT_MAX_SIZE,       32 * 1024 * 1024 + 1, true     },
        };

        const int NUM_DATA = sizeof DATA / sizeof *DATA;

        for (int i = 0; i != NUM_DATA; ++i) {
            const int  LINE       = DATA[i].d_lineNum;
            const int  MAX_SIZE   = DATA[i].d_maxSize;
            const int  SIZE       = DATA[i].d_size;
            const bool CAN_DECODE = DATA[i].d_canDecode;

            if (veryVeryVerbose) {
                P_(LINE) P_(MAX_SIZE) P_(SIZE) P(CAN_DECODE)
            }

            const bsl::vector<char> DATA(SIZE, '\xAB');

            bdlsb::MemOutStreamBuf osb;
            balber::BerEncoder encoder;

            int rc = encoder.encode(&osb, DATA);
            ASSERT(0 == rc);

            balber::BerDecoderOptions decoderOptions;
            if (MAX_SIZE != DEFAULT_MAX_SIZE) {
                decoderOptions.setMaxSequenceSize(MAX_SIZE);
            }

            balber::BerDecoder decoder(&decoderOptions);

            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());

            if (CAN_DECODE) {
                bsl::vector<char> decodedData;
                rc = decoder.decode(&isb, &decodedData);
                ASSERT(0 == rc);
                ASSERT(DATA == decodedData);
            }
            else {
                bsl::vector<char> decodedData;
                rc = decoder.decode(&isb, &decodedData);
                ASSERT(0 != rc);
            }
        }
        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 19: {
        // --------------------------------------------------------------------
        // TESTING decoding complex types with default elements
        //
        // Concerns:
        //: 1 The "DefaultEmptyStrings" decoder option instructs a BER decoder
        //:   to load the default value of elements only when it is true,
        //:   the decoder reads an empty string, and the attribute has a
        //:   default string value.
        //:
        //: 2 The "DefaultEmptyStrings" decoder option does not affect whether
        //:   or not a BER decoder loads the default value of non-string
        //:   types.
        //:
        //: 3 The "DefaultEmptyStrings" decoder option applies to elements of
        //:   both sequences and choices.
        //
        // Plan:
        //: 1 Create a choice type having an integer, string, and enumeration
        //:   selection, each having a default value.
        //:
        //: 2 Perform a depth-ordered enumeration of encoding and then
        //:   decoding each permutation of an instance of the choice type
        //:   having a zero, unset, default, or arbitary value.
        //:
        //: 3 Validate that if the encoded object was an empty string
        //:   selection, the decoded object is loaded with the default value
        //:   if the "DefaultEmptyStrings" decoder option is set, and otherwise
        //:   the decoded object is loaded with an empty string value.
        //:
        //: 4 Create a sequence type having integer, string, and enumeration
        //:   attributes, each having a default value.
        //:
        //: 5 Perform a locally depth-ordered enumeration of encoding and
        //:   an instance of the sequence type, testing each attribute in
        //:   isolation.
        //:
        //: 6 For each attribute test, validate that the if the string
        //:   attribute of the encoded object was empty, the decoded object
        //:   is loaded with the default value if the "DefaultEmptyStrings"
        //:   decoder option is set, and otherwise the decoded attribute is
        //:   loaded with an empty string value.
        //
        // Testing:
        //   Given 'TYPE' is a choice or sequence having selections or
        //   attributes with default values, respectively:
        //
        //   int BerDecoder::decode(bsl::streambuf *streamBuf, TYPE *variable)
        //   int BerDeocder::decode(bsl::istream&   stream   , TYPE *variable)
        // --------------------------------------------------------------------

        if (verbose)
            bsl::cout
                << "\nTESTING decoding complex types with default elements"
                << "\n===================================================="
                << bsl::endl;

        if (veryVerbose)
            bsl::cout
                << "\nTest decoding choices whose selections have defaults"
                << "\n----------------------------------------------------"
                << bsl::endl;
        {
            enum Selection {
                INT,
                STRING,
                ENUM
            };

            enum Value {
                ZERO,
                UNSET,
                DEFAULT,
                ARBITRARY
            };

            typedef test::MyEnumeration::Value Enum;

            static const int  ARBITRARY_INT      = 42;
            static const char ARBITRARY_STRING[] = "I'm arbitrary!";
            static const Enum ARBITRARY_ENUM     = test::MyEnumeration::VALUE1;

            static const bool NO  = false;
            static const bool YES = true;

            static const struct {
                int       d_line;
                bool      d_defaultStrings;
                Selection d_selection;
                Value     d_encodedValue;
                Value     d_decodedValue;
            } DATA[] = {
                //LINE DEFAULT STRINGS SELECTION ENCODED VALUE DECODED VALUE
                //---- --------------- --------- ------------- -------------
                {  L_ , NO            , INT     , ZERO        , ZERO        },
                {  L_ , NO            , INT     , UNSET       , DEFAULT     },
                {  L_ , NO            , INT     , DEFAULT     , DEFAULT     },
                {  L_ , NO            , INT     , ARBITRARY   , ARBITRARY   },
                {  L_ , NO            , STRING  , ZERO        , ZERO        },
                {  L_ , NO            , STRING  , UNSET       , DEFAULT     },
                {  L_ , NO            , STRING  , DEFAULT     , DEFAULT     },
                {  L_ , NO            , STRING  , ARBITRARY   , ARBITRARY   },
                {  L_ , NO            , ENUM    , ZERO        , ZERO        },
                {  L_ , NO            , ENUM    , UNSET       , DEFAULT     },
                {  L_ , NO            , ENUM    , DEFAULT     , DEFAULT     },
                {  L_ , NO            , ENUM    , ARBITRARY   , ARBITRARY   },
                {  L_ , YES           , INT     , ZERO        , ZERO        },
                {  L_ , YES           , INT     , UNSET       , DEFAULT     },
                {  L_ , YES           , INT     , DEFAULT     , DEFAULT     },
                {  L_ , YES           , INT     , ARBITRARY   , ARBITRARY   },
                {  L_ , YES           , STRING  , ZERO        , DEFAULT     },
                {  L_ , YES           , STRING  , UNSET       , DEFAULT     },
                {  L_ , YES           , STRING  , DEFAULT     , DEFAULT     },
                {  L_ , YES           , STRING  , ARBITRARY   , ARBITRARY   },
                {  L_ , YES           , ENUM    , ZERO        , ZERO        },
                {  L_ , YES           , ENUM    , UNSET       , DEFAULT     },
                {  L_ , YES           , ENUM    , DEFAULT     , DEFAULT     },
                {  L_ , YES           , ENUM    , ARBITRARY   , ARBITRARY   }
            };

            static const int NUM_DATA = sizeof(DATA) / sizeof(DATA[0]);

            for (int i = 0; i != NUM_DATA; ++i) {
                const int       LINE            = DATA[i].d_line;
                const bool      DEFAULT_STRINGS = DATA[i].d_defaultStrings;
                const Selection SELECTION       = DATA[i].d_selection;
                const Value     ENCODED_VALUE   = DATA[i].d_encodedValue;
                const Value     DECODED_VALUE   = DATA[i].d_decodedValue;

                typedef test::MyChoiceWithDefaultValues SerializableObject;

                SerializableObject object(&ta);

                switch (SELECTION) {
                  case INT: {
                    switch (ENCODED_VALUE) {
                      case ZERO: {
                        object.makeSelection0(0);
                      } break;
                      case UNSET: {
                        object.makeSelection0();
                      } break;
                      case DEFAULT: {
                        object.makeSelection0(
                            SerializableObject::DEFAULT_INITIALIZER_SELECTION0);
                      } break;
                      case ARBITRARY: {
                        object.makeSelection0(ARBITRARY_INT);
                      } break;
                    }
                  } break;
                  case STRING: {
                    switch (ENCODED_VALUE) {
                      case ZERO: {
                        object.makeSelection1("");
                      } break;
                      case UNSET: {
                        object.makeSelection1();
                      } break;
                      case DEFAULT: {
                        object.makeSelection1(
                            SerializableObject::DEFAULT_INITIALIZER_SELECTION1);
                      } break;
                      case ARBITRARY: {
                        object.makeSelection1(ARBITRARY_STRING);
                      } break;
                    }
                  } break;
                  case ENUM: {
                    switch (ENCODED_VALUE) {
                      case ZERO: {
                        object.makeSelection2(test::MyEnumeration::VALUE1);
                      } break;
                      case UNSET: {
                        object.makeSelection2();
                      } break;
                      case DEFAULT: {
                        object.makeSelection2(
                            SerializableObject::DEFAULT_INITIALIZER_SELECTION2);
                      } break;
                      case ARBITRARY: {
                        object.makeSelection2(ARBITRARY_ENUM);
                      } break;
                    }
                  } break;
                }

                balber::BerEncoderOptions encoderOptions;
                balber::BerEncoder encoder(&encoderOptions, &ta);

                bdlsb::MemOutStreamBuf outStreamBuf(&ta);
                int rc = encoder.encode(&outStreamBuf, object);
                ASSERTV(rc, 0 == rc);

                bdlsb::FixedMemInStreamBuf inStreamBuf(outStreamBuf.data(),
                                                       outStreamBuf.length());

                balber::BerDecoderOptions decoderOptions;
                if (!DEFAULT_STRINGS) {
                    decoderOptions.setDefaultEmptyStrings(false);
                }
                balber::BerDecoder decoder(&decoderOptions, &ta);

                SerializableObject decodedObject;
                rc = decoder.decode(&inStreamBuf, &decodedObject);
                ASSERTV(rc, 0 == rc);

                switch (SELECTION) {
                  case INT: {
                    ASSERTV(decodedObject.selectionId(),
                            decodedObject.isSelection0Value());
                    switch (DECODED_VALUE) {
                      case ZERO: {
                        ASSERT1_EQ(LINE, 0, decodedObject.selection0());
                      } break;
                      case UNSET: {
                        ASSERT1_EQ(LINE, 0, decodedObject.selection0());
                      } break;
                      case DEFAULT: {
                        ASSERT1_EQ(
                            LINE,
                            SerializableObject::DEFAULT_INITIALIZER_SELECTION0,
                            decodedObject.selection0());
                      } break;
                      case ARBITRARY: {
                        ASSERT1_EQ(
                            LINE, ARBITRARY_INT, decodedObject.selection0());
                      } break;
                    }
                  } break;
                  case STRING: {
                    ASSERTV(decodedObject.selectionId(),
                            decodedObject.isSelection1Value());
                    switch (DECODED_VALUE) {
                      case ZERO: {
                        ASSERT1_EQ(LINE, "", decodedObject.selection1());
                      } break;
                      case UNSET: {
                        ASSERT1_EQ(LINE, "", decodedObject.selection1());
                      } break;
                      case DEFAULT: {
                        ASSERT1_EQ(
                            LINE,
                            SerializableObject::DEFAULT_INITIALIZER_SELECTION1,
                            decodedObject.selection1());
                      } break;
                      case ARBITRARY: {
                        ASSERT1_EQ(LINE,
                                   ARBITRARY_STRING,
                                   decodedObject.selection1());
                      } break;
                    }
                  } break;
                  case ENUM: {
                    ASSERTV(decodedObject.selectionId(),
                            decodedObject.isSelection2Value());
                    switch (DECODED_VALUE) {
                      case ZERO: {
                        ASSERT1_EQ(LINE, 0, decodedObject.selection2());
                      } break;
                      case UNSET: {
                        ASSERT1_EQ(LINE, 0, decodedObject.selection2());
                      } break;
                      case DEFAULT: {
                        ASSERT1_EQ(
                            LINE,
                            SerializableObject::DEFAULT_INITIALIZER_SELECTION2,
                            decodedObject.selection2());
                      } break;
                      case ARBITRARY: {
                        ASSERT1_EQ(
                            LINE, ARBITRARY_ENUM, decodedObject.selection2());
                      } break;
                    }
                  } break;
                }
            }
        }

        if (veryVerbose)
            bsl::cout
                << "\nTest decoding sequences whose attributes have defaults"
                << "\n------------------------------------------------------"
                << bsl::endl;
        {
            enum Value {
                ZERO,
                UNSET,
                DEFAULT,
                ARBITRARY
            };

            static const Value ARB = ARBITRARY;

            typedef test::MyEnumeration::Value Enum;

            static const int  ARBITRARY_INT      = 42;
            static const char ARBITRARY_STRING[] = "I'm arbitrary";
            static const Enum ARBITRARY_ENUM     = test::MyEnumeration::VALUE1;

            static const bool N = false;
            static const bool Y = true;

            static const struct {
                int   d_line;
                bool  d_defaultStrings;
                Value d_encodedAttr0;
                Value d_encodedAttr1;
                Value d_encodedAttr2;
                Value d_decodedAttr0;
                Value d_decodedAttr1;
                Value d_decodedAttr2;
            } DATA[] = {
                //LN D  ATTR 0   ATTR 1   ATTR 2  DEC A 0  DEC A 1  DEC A 2
                //-- - -------- -------- -------- -------- -------- --------
                {L_, Y, UNSET  , UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, UNSET  , UNSET  , ZERO   , DEFAULT, DEFAULT, ZERO   },
                {L_, Y, UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, UNSET  , UNSET  , ARB    , DEFAULT, DEFAULT, ARB    },

                {L_, Y, UNSET  , UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, UNSET  , ZERO   , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, UNSET  , DEFAULT, UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, UNSET  , ARB    , UNSET  , DEFAULT, ARB    , DEFAULT},

                {L_, Y, UNSET  , UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, ZERO   , UNSET  , UNSET  , ZERO   , DEFAULT, DEFAULT},
                {L_, Y, DEFAULT, UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, Y, ARB    , UNSET  , UNSET  , ARB    , DEFAULT, DEFAULT},

                {L_, N, UNSET  , UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, N, UNSET  , UNSET  , ZERO   , DEFAULT, DEFAULT, ZERO   },
                {L_, N, UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT, DEFAULT},
                {L_, N, UNSET  , UNSET  , ARB    , DEFAULT, DEFAULT, ARB    },

                {L_, N, UNSET  , UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, N, UNSET  , ZERO   , UNSET  , DEFAULT, ZERO   , DEFAULT},
                {L_, N, UNSET  , DEFAULT, UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, N, UNSET  , ARB    , UNSET  , DEFAULT, ARB    , DEFAULT},

                {L_, N, UNSET  , UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, N, ZERO   , UNSET  , UNSET  , ZERO   , DEFAULT, DEFAULT},
                {L_, N, DEFAULT, UNSET  , UNSET  , DEFAULT, DEFAULT, DEFAULT},
                {L_, N, ARB    , UNSET  , UNSET  , ARB    , DEFAULT, DEFAULT},
            };

            static const int NUM_DATA = sizeof(DATA) / sizeof(DATA[0]);

            for (int i = 0; i != NUM_DATA; ++i) {
                const int   LINE               = DATA[i].d_line;
                const bool  DEFAULT_STRINGS    = DATA[i].d_defaultStrings;
                const Value ENCODED_ATTRIBUTE0 = DATA[i].d_encodedAttr0;
                const Value ENCODED_ATTRIBUTE1 = DATA[i].d_encodedAttr1;
                const Value ENCODED_ATTRIBUTE2 = DATA[i].d_encodedAttr2;
                const Value DECODED_ATTRIBUTE0 = DATA[i].d_decodedAttr0;
                const Value DECODED_ATTRIBUTE1 = DATA[i].d_decodedAttr1;
                const Value DECODED_ATTRIBUTE2 = DATA[i].d_decodedAttr2;

                typedef test::MySequenceWithDefaultValues SerializableObject;

                SerializableObject object(&ta);

                switch (ENCODED_ATTRIBUTE0) {
                  case ZERO: {
                    object.attribute0() = 0;
                  } break;
                  case UNSET: {
                    // do nothing
                  } break;
                  case DEFAULT: {
                    object.attribute0() = SerializableObject::
                    DEFAULT_INITIALIZER_ATTRIBUTE0;
                  } break;
                  case ARBITRARY: {
                    object.attribute0() = ARBITRARY_INT;
                  } break;
                }

                switch (ENCODED_ATTRIBUTE1) {
                  case ZERO: {
                    object.attribute1().clear();
                  } break;
                  case UNSET: {
                    // do nothing
                  } break;
                  case DEFAULT: {
                    object.attribute1() = SerializableObject::
                    DEFAULT_INITIALIZER_ATTRIBUTE1;
                  } break;
                  case ARBITRARY: {
                    object.attribute1() = ARBITRARY_STRING;
                  } break;
                }

                switch (ENCODED_ATTRIBUTE2) {
                  case ZERO: {
                    object.attribute2() =
                        static_cast<test::MyEnumeration::Value>(0);
                  } break;
                  case UNSET: {
                    // do nothing
                  } break;
                  case DEFAULT: {
                    object.attribute2() = SerializableObject::
                    DEFAULT_INITIALIZER_ATTRIBUTE2;
                  } break;
                  case ARBITRARY: {
                    object.attribute2() = ARBITRARY_ENUM;
                  } break;
                }

                balber::BerEncoderOptions encoderOptions;
                balber::BerEncoder        encoder(&encoderOptions, &ta);

                bdlsb::MemOutStreamBuf outStreamBuf(&ta);
                int rc = encoder.encode(&outStreamBuf, object);
                ASSERTV(LINE, rc, 0 == rc);

                bdlsb::FixedMemInStreamBuf inStreamBuf(outStreamBuf.data(),
                                                       outStreamBuf.length());


                balber::BerDecoderOptions decoderOptions;
                if (!DEFAULT_STRINGS) {
                    decoderOptions.setDefaultEmptyStrings(false);
                }
                balber::BerDecoder decoder(&decoderOptions, &ta);

                SerializableObject decodedObject;
                rc = decoder.decode(&inStreamBuf, &decodedObject);
                ASSERTV(LINE, rc, 0 == rc);

                switch (DECODED_ATTRIBUTE0) {
                  case ZERO: {
                    ASSERT1_EQ(LINE, 0, decodedObject.attribute0());
                  } break;
                  case UNSET: {
                      ASSERT1_EQ(LINE, 0, decodedObject.attribute0());
                  } break;
                  case DEFAULT: {
                    ASSERT1_EQ(
                        LINE,
                        SerializableObject::DEFAULT_INITIALIZER_ATTRIBUTE0,
                        object.attribute0());
                  } break;
                  case ARBITRARY: {
                    ASSERT1_EQ(LINE, ARBITRARY_INT, object.attribute0());
                  } break;
                }

                switch (DECODED_ATTRIBUTE1) {
                  case ZERO: {
                    ASSERT1_EQ(LINE, "", decodedObject.attribute1());
                  } break;
                  case UNSET: {
                    ASSERT1_EQ(LINE, "", decodedObject.attribute1());
                  } break;
                  case DEFAULT: {
                    ASSERT1_EQ(
                        LINE,
                        SerializableObject::DEFAULT_INITIALIZER_ATTRIBUTE1,
                        decodedObject.attribute1());
                  } break;
                  case ARBITRARY: {
                    ASSERT1_EQ(
                        LINE, ARBITRARY_STRING, decodedObject.attribute1());
                  } break;
                }

                switch (DECODED_ATTRIBUTE2) {
                  case ZERO: {
                    ASSERT1_EQ(LINE, 0, decodedObject.attribute2());
                  } break;
                  case UNSET: {
                    ASSERT1_EQ(LINE, 0, decodedObject.attribute2());
                  } break;
                  case DEFAULT: {
                    ASSERT1_EQ(
                        LINE,
                        SerializableObject::DEFAULT_INITIALIZER_ATTRIBUTE2,
                        object.attribute2());
                  } break;
                  case ARBITRARY: {
                    ASSERT1_EQ(LINE, ARBITRARY_ENUM, object.attribute2());
                  } break;
                }
            }
        }
        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 18: {
        // --------------------------------------------------------------------
        // TESTING encoding & decoding for RawData
        //
        // Concerns:
        //: 1 A 'vector<char>' with certain formatting modes will be encoded
        //:   by 'BerEncoder' as a primitive array.
        //:
        //: 2 A 'vector<char>' with any other formatting mode, or a
        //:   'vector<unsigned char>', will be encoded by 'BerEncoder' as a
        //:   sequence of individual values.
        //:
        //: 3 'BerDecoder' when decoding a 'vecctor<char>' or a
        //:   'vector<unsigned char>' should be able to decode either of those
        //:   formats, independant of which encoding choice was made.
        //:
        //: 4 Similarly, since 'char' and 'unsigned char' are generally really
        //:   used interchangably as 'byte', the type when encoded shouldn't
        //:   prevent decoding even if it does not match the type when
        //:   decoding.
        //
        // Plan:
        //: 1 For each of the differently structured 'RawData' types from the
        //:   test schema ('test::RawData', 'test::RawDataSwitched',
        //:   'test::RawDataUnformatted') populate an object with test data and
        //:   encode it.
        //:
        //: 2 Use a 'BerDecoder' to decode into a 'test::RawData'.
        //:
        //: 3 Verify that the 2 vectors from the decoded object match the
        //:   test data that was populated into the encoded object.
        //
        // Testing:
        //   int BerDecoder_Node::decode(bsl::vector<char>          *variable,
        //                               bdlat_TypeCategory::Array  )
        //   int BerDecoder_Node::decode(bsl::vector<unsigned char> *variable,
        //                               bdlat_TypeCategory::Array  )
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTESTING encoding & decoding for RawData"
                               << "\n======================================="
                               << bsl::endl;

        const char TEST_DATA[] = "300C8001 22A10082 0548656C 6C6F";
        bsl::vector<char> testData = loadFromHex(TEST_DATA);
        bsl::vector<unsigned char> utestData;
        bsl::copy(testData.begin(), testData.end(),
                                    bsl::back_inserter(utestData));

        ASSERT(testData.size() == utestData.size());

        if (verbose) bsl::cout << "\nTesting with rawdata." << bsl::endl;
        {
            test::RawData rawData;
            rawData.charvec() = testData;
            bsl::copy(testData.begin(),testData.end(),
                bsl::back_inserter(rawData.ucharvec()));

            ASSERT(testData == rawData.charvec());
            ASSERT(utestData == rawData.ucharvec());

            balber::BerEncoderOptions encoderOptions;
            balber::BerEncoder encoder(&encoderOptions);
            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, rawData));

            if (veryVerbose) {
                P(osb.length());
                printBuffer(osb.data(),osb.length());
            }

            balber::BerDecoderOptions decoderOptions;
            balber::BerDecoder decoder(&decoderOptions);
            test::RawData rawDataParsed;
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &rawDataParsed));

            ASSERT(rawData == rawDataParsed);
            ASSERT(testData == rawDataParsed.charvec());
            ASSERT(utestData == rawDataParsed.ucharvec());
        }

        if (verbose) bsl::cout << "\nTesting with rawdataunformatted."
           << bsl::endl;
        {
            test::RawDataUnformatted rawDataUf;
            rawDataUf.charvec() = testData;
            bsl::copy(testData.begin(),testData.end(),
                bsl::back_inserter(rawDataUf.ucharvec()));

            ASSERT(testData == rawDataUf.charvec());
            ASSERT(utestData == rawDataUf.ucharvec());

            balber::BerEncoderOptions encoderOptions;
            balber::BerEncoder encoder(&encoderOptions);
            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, rawDataUf));

            if (veryVerbose) {
                P(osb.length());
                printBuffer(osb.data(),osb.length());
            }

            balber::BerDecoderOptions decoderOptions;
            balber::BerDecoder decoder(&decoderOptions);
            test::RawData rawDataParsed;
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &rawDataParsed));

            ASSERT(testData == rawDataParsed.charvec());
            ASSERT(utestData == rawDataParsed.ucharvec());
        }

        if (verbose) bsl::cout << "\nTesting uchar->char with rawdataswitched."
           << bsl::endl;
        {
            test::RawDataSwitched rawDataSw;
            bsl::copy(testData.begin(),testData.end(),
                bsl::back_inserter(rawDataSw.ucharvec()));

            ASSERT(0 == rawDataSw.charvec().size());
            ASSERT(utestData == rawDataSw.ucharvec());

            balber::BerEncoderOptions encoderOptions;
            balber::BerEncoder encoder(&encoderOptions);
            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, rawDataSw));

            if (veryVerbose) {
                P(osb.length());
                printBuffer(osb.data(),osb.length());
            }

            balber::BerDecoderOptions decoderOptions;
            balber::BerDecoder decoder(&decoderOptions);
            test::RawData rawDataParsed;
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            int rc = decoder.decode(&isb,&rawDataParsed);
            if (rc != 0)
            {
                bsl::cerr << "Errors parsing:" <<
                          decoder.loggedMessages() << bsl::endl;
                ASSERT(rc == 0);
            }

            ASSERT(0 == rawDataParsed.ucharvec().size());
            ASSERT(testData == rawDataParsed.charvec());
        }

      } break;
      case 17: {
        // --------------------------------------------------------------------
        // TESTING decoding for date/time components using a variant
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        // --------------------------------------------------------------------

        if (verbose) bsl::cout
                            << "\nTESTING decoding for date/time using variant"
                            << "\n============================================"
                            << bsl::endl;

        balber::BerEncoderOptions options;
        options.setEncodeDateAndTimeTypesAsBinary(true);
        const balber::BerEncoderOptions DEFOPTS;

        if (verbose) bsl::cout << "\nDefine data" << bsl::endl;

        static const struct {
            int d_lineNum;   // source line number
            int d_year;      // year under test
            int d_month;     // month under test
            int d_day;       // day under test
            int d_hour;      // hour under test
            int d_minutes;   // minutes under test
            int d_seconds;   // seconds under test
            int d_milliSecs; // milli seconds under test
            int d_tzoffset;  // time zone offset
        } DATA[] = {
   //line no.  year   month   day   hour    min   sec    ms  offset
   //-------   -----  -----   ---   ----    ---   ---    --  ------
    {      L_,      1,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,    1,     0,     0,    0,    0,     45     },
    {      L_,      1,     1,    1,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     1,    1,     1,     1,    1,    1,      0     },
    {      L_,      1,     1,    1,     1,     1,    1,    1,    500     },
    {      L_,      1,     1,    1,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     1,    1,     1,    23,   59,   59,      0     },
    {      L_,      1,     1,    1,     1,    23,   59,   59,   1439     },
    {      L_,      1,     1,    1,     1,    23,   59,   59,  -1439     },

    {      L_,      1,     1,    2,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,    2,     0,     0,    0,    0,   1439     },
    {      L_,      1,     1,    2,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     1,    2,     1,     1,    1,    1,      0     },
    {      L_,      1,     1,    2,     1,     1,    1,    1,    500     },

    {      L_,      1,     1,    2,     1,    23,   59,   59,      0     },
    {      L_,      1,     1,    2,     1,    23,   59,   59,    500     },
    {      L_,      1,     1,    2,     1,    23,   59,   59,   -500     },

    {      L_,      1,     1,   10,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,   10,     1,     1,    1,    1,     99     },

    {      L_,      1,     1,   30,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,   31,     0,     0,    0,    0,   1439     },
    {      L_,      1,     1,   31,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     2,    1,     0,     0,    0,    0,      0     },
    {      L_,      1,     2,    1,    23,    59,   59,    0,   1439     },

    {      L_,      1,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,      1,    12,   31,    23,    59,   59,    0,   1439     },

    {      L_,      2,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,      2,     1,    1,    23,    59,   59,    0,   1439     },

    {      L_,      4,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,      4,     1,    1,    23,    59,   59,    0,   1439     },

    {      L_,      4,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,      4,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,      4,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,      4,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,      4,     2,   29,    23,    59,   59,    0,   1439     },
    {      L_,      4,     2,   29,    23,    59,   59,    0,  -1439     },

    {      L_,      4,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,      4,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,      4,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,      8,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,      8,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,      8,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,      8,     2,   29,    23,    59,   59,    0,   1439     },

    {      L_,      8,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,      8,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,    100,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    100,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,    100,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,    100,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    100,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,    100,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,    400,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    400,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,    400,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,    400,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,    400,     2,   29,    23,    59,   59,    0,   1439     },
    {      L_,    400,     2,   29,    23,    59,   59,    0,  -1439     },

    {      L_,    400,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    400,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,    400,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,    500,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    500,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,    500,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    500,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,    800,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    800,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,    800,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,    800,     2,   29,    23,    59,   59,    0,   1439     },

    {      L_,    800,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    800,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,   1000,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   1000,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,   1000,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   1000,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,   2000,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   2000,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,   2000,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,   2000,     2,   29,    23,    59,   59,    0,   1439     },

    {      L_,   2000,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   2000,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,   2016,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   2017,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   2018,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   2019,    12,   31,     0,     0,    0,    0,      0     },

    {      L_,   2020,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,   2020,     1,    1,     0,     0,    0,    0,   1439     },
    {      L_,   2020,     1,    1,     0,     0,    0,    0,  -1439     },

    {      L_,   2020,     1,    1,    23,    59,   59,  999,      0     },
    {      L_,   2020,     1,    1,    23,    59,   59,  999,   1439     },
    {      L_,   2020,     1,    1,    23,    59,   59,  999,  -1439     },

    {      L_,   2020,     1,    2,     0,     0,    0,    0,      0     },
    {      L_,   2020,     1,    2,     0,     0,    0,    0,   1439     },
    {      L_,   2020,     1,    2,     0,     0,    0,    0,  -1439     },

    {      L_,   2020,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   2020,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,   2020,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,   2020,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,   2020,     2,   29,    23,    59,   59,    0,   1439     },
    {      L_,   2020,     2,   29,    23,    59,   59,    0,  -1439     },

    {      L_,   2020,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   2020,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,   2020,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,   2021,     1,    2,     0,     0,    0,    0,      0     },
    {      L_,   2022,     1,    2,     0,     0,    0,    0,      0     },

    {      L_,   9999,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   9999,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,   9999,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,   9999,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   9999,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,   9999,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,   9999,    12,   30,     0,     0,    0,    0,      0     },
    {      L_,   9999,    12,   30,    23,    59,   59,    0,   1439     },

    {      L_,   9999,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   9999,    12,   31,    23,    59,   59,    0,   1439     },
        };
        const int NUM_DATA = sizeof DATA / sizeof *DATA;

        if (verbose) bsl::cout << "\nTesting 'bdlt::Date'." << bsl::endl;
        {
            typedef bdlb::Variant2<bdlt::Date, bdlt::DateTz> Variant;

            typedef bdlt::Date Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;

                if (veryVerbose) { P_(Y) P_(M) P(D) }

                const Type VALUE(Y, M, D);
                Variant    value1, value2;

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value1));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value1.is<bdlt::Date>());
                    LOOP3_ASSERT(LINE, VALUE, value1,
                                 VALUE == value1.the<bdlt::Date>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value2));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value2.is<bdlt::Date>());
                    LOOP3_ASSERT(LINE, VALUE, value2,
                                 VALUE == value2.the<bdlt::Date>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value2);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::DateTz'." << bsl::endl;
        {
            typedef bdlb::Variant2<bdlt::Date, bdlt::DateTz> Variant;

            typedef bdlt::DateTz Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;
                const int OFF  = DATA[i].d_tzoffset;

                const Type VALUE(bdlt::Date(Y, M, D), OFF);
                Variant    value1, value2;

                if (veryVerbose) { P_(Y) P_(M) P_(D) P(OFF) }

                if (OFF) {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value1));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value1.is<bdlt::DateTz>());
                    LOOP3_ASSERT(LINE, VALUE, value1,
                                 VALUE == value1.the<bdlt::DateTz>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value2));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value2.is<bdlt::DateTz>());
                    LOOP3_ASSERT(LINE, VALUE, value2,
                                 VALUE == value2.the<bdlt::DateTz>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::Time'." << bsl::endl;
        {
            typedef bdlb::Variant2<bdlt::Time, bdlt::TimeTz> Variant;

            typedef bdlt::Time Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;

                if (veryVerbose) { P_(H) P_(MM) P_(S) P(MS) }

                const Type VALUE(H, MM, S, MS);
                Variant    value1, value2;

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value1));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value1.is<bdlt::Time>());
                    LOOP3_ASSERT(LINE, VALUE, value1,
                                 VALUE == value1.the<bdlt::Time>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value2));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value2.is<bdlt::Time>());
                    LOOP3_ASSERT(LINE, VALUE, value2,
                                 VALUE == value2.the<bdlt::Time>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value2);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::TimeTz'." << bsl::endl;
        {
            typedef bdlb::Variant2<bdlt::Time, bdlt::TimeTz> Variant;

            typedef bdlt::TimeTz Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;
                const int OFF  = DATA[i].d_tzoffset;

                const Type VALUE(bdlt::Time(H, MM, S, MS), OFF);
                Variant    value1, value2;

                if (veryVerbose) { P_(H) P_(MM) P_(S) P_(MS) P(OFF) }

                if (OFF) {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value1));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value1.is<bdlt::TimeTz>());
                    LOOP3_ASSERT(LINE, VALUE, value1,
                                 VALUE == value1.the<bdlt::TimeTz>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value2));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value2.is<bdlt::TimeTz>());
                    LOOP3_ASSERT(LINE, VALUE, value2,
                                 VALUE == value2.the<bdlt::TimeTz>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::Datetime'." << bsl::endl;
        {
            typedef bdlb::Variant2<bdlt::Datetime, bdlt::DatetimeTz> Variant;

            typedef bdlt::Datetime Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;

                const Type VALUE(Y, M, D, H, MM, S, MS);
                Variant    value1, value2;

                if (veryVerbose) { P_(Y) P_(M) P_(D) P_(H)
                                   P_(MM) P_(S) P(MS) }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (osb.length() > 6) {
                        // Datetime objects having length greater that 6 bytes
                        // are always encoded with a time zone.

                        continue;
                    }

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value1));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value1.is<bdlt::Datetime>());
                    LOOP3_ASSERT(LINE, VALUE, value1,
                                 VALUE == value1.the<bdlt::Datetime>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value2));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value2.is<bdlt::Datetime>());
                    LOOP3_ASSERT(LINE, VALUE, value2,
                                 VALUE == value2.the<bdlt::Datetime>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value2);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::DatetimeTz'." << bsl::endl;
        {
            typedef bdlb::Variant2<bdlt::Datetime, bdlt::DatetimeTz> Variant;

            typedef bdlt::DatetimeTz Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;
                const int OFF  = DATA[i].d_tzoffset;

                const Type VALUE(bdlt::Datetime(Y, M, D, H, MM, S, MS), OFF);
                Variant    value1, value2;

                if (veryVerbose) { P_(Y) P_(M) P_(D) P_(H)
                                   P_(MM) P_(S) P_(MS) P(OFF) }

                if (OFF) {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value1));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value1.is<bdlt::DatetimeTz>());
                    LOOP3_ASSERT(LINE, VALUE, value1,
                                 VALUE == value1.the<bdlt::DatetimeTz>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value1);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value2));
                    printDiagnostic(decoder);

                    LOOP_ASSERT(LINE, value2.is<bdlt::DatetimeTz>());
                    LOOP3_ASSERT(LINE, VALUE, value2,
                                 VALUE == value2.the<bdlt::DatetimeTz>());
                    if (veryVerbose) {
                        P(VALUE);
                        P(value2);
                    }
                }
            }
        }
      } break;
      case 16: {
        // --------------------------------------------------------------------
        // TESTING encoding & decoding for date/time components
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTESTING encoding & decoding for date/time"
                               << "\n========================================="
                               << bsl::endl;

        balber::BerEncoderOptions options;
        options.setEncodeDateAndTimeTypesAsBinary(true);
        const balber::BerEncoderOptions DEFOPTS;

        if (verbose) bsl::cout << "\nTesting Date Brute force." << bsl::endl;
        {
            const int YEARS[] = { 1, 4, 96, 100, 400, 500, 800, 1000, 1600,
                                  1700, 1751, 1752, 1753, 1930, 2000, 2010,
                                  2012, 2019, 2020, 2021, 6478, 6479, 6480,
                                  9998, 9999 };
            const int NUM_YEARS = sizeof YEARS / sizeof *YEARS;

            const int MONTHS[] = { 1, 2, 5, 8, 9, 12 };
            const int NUM_MONTHS = sizeof MONTHS / sizeof *MONTHS;

            const int DAYS[] = { 1, 2, 5, 10, 15, 20, 28, 29, 30, 31 };
            const int NUM_DAYS = sizeof DAYS / sizeof *DAYS;

            for (int i = 0; i < NUM_YEARS; ++i) {
            for (int j = 0; j < NUM_MONTHS; ++j) {
            for (int k = 0; k < NUM_DAYS; ++k) {

                const int YEAR  = YEARS[i];
                const int MONTH = MONTHS[j];
                const int DAY   = DAYS[k];

                if (bdlt::Date::isValidYearMonthDay(YEAR, MONTH, DAY)
                 && bdlt::ProlepticDateImpUtil::isValidYearMonthDay(YEAR,
                                                                    MONTH,
                                                                    DAY)) {

                    if (veryVerbose) { P_(YEAR) P_(MONTH) P(DAY) }

                    const bdlt::Date VALUE(YEAR, MONTH, DAY); bdlt::Date value;
                    const int OFF1 = 0, OFF2 =-1439, OFF3 =1439;
                    const bdlt::DateTz VALUE1(bdlt::Date(YEAR, MONTH, DAY),
                                             OFF1);
                    const bdlt::DateTz VALUE2(bdlt::Date(YEAR, MONTH, DAY),
                                             OFF2);
                    const bdlt::DateTz VALUE3(bdlt::Date(YEAR, MONTH, DAY),
                                             OFF3);
                    bdlt::DateTz value1, value2, value3;

                    {
                        bdlsb::MemOutStreamBuf osb;
                        balber::BerEncoder encoder(&DEFOPTS);
                        ASSERT(0 == encoder.encode(&osb, VALUE));

                        if (veryVerbose) {
                            P(osb.length());
                            printBuffer(osb.data(), osb.length());
                        }

                        bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                       osb.length());
                        ASSERT(0 == decoder.decode(&isb, &value));
                        printDiagnostic(decoder);

                        LOOP2_ASSERT(VALUE, value, VALUE == value);
                        if (veryVerbose) {
                            P(VALUE);
                            P(value);
                        }
                    }

                    {
                        bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                        balber::BerEncoder encoder(&DEFOPTS);
                        ASSERT(0 == encoder.encode(&osb1, VALUE1));
                        ASSERT(0 == encoder.encode(&osb2, VALUE2));
                        ASSERT(0 == encoder.encode(&osb3, VALUE3));

                        if (veryVerbose) {
                            P(osb1.length());
                            P(osb2.length());
                            P(osb3.length());
                            printBuffer(osb1.data(), osb1.length());
                            printBuffer(osb2.data(), osb2.length());
                            printBuffer(osb3.data(), osb3.length());
                        }

                        bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                       osb1.length());
                        bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                       osb2.length());
                        bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                       osb3.length());

                        ASSERT(0 == decoder.decode(&isb1, &value1));
                        ASSERT(0 == decoder.decode(&isb2, &value2));
                        ASSERT(0 == decoder.decode(&isb3, &value3));
                        printDiagnostic(decoder);

                        LOOP2_ASSERT(VALUE1, value1, VALUE1 == value1);
                        LOOP2_ASSERT(VALUE2, value2, VALUE2 == value2);
                        LOOP2_ASSERT(VALUE3, value3, VALUE3 == value3);
                        if (veryVerbose) {
                            P(VALUE1); P(value1);
                            P(VALUE2); P(value2);
                            P(VALUE3); P(value3);
                        }
                    }

                    {
                        bdlsb::MemOutStreamBuf osb;
                        balber::BerEncoder encoder(&DEFOPTS);
                        ASSERT(0 == encoder.encode(&osb, VALUE));

                        if (veryVerbose) {
                            P(osb.length());
                            printBuffer(osb.data(), osb.length());
                        }

                        bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                      osb.length());
                        ASSERT(0 == decoder.decode(&isb, &value));
                        printDiagnostic(decoder);

                        LOOP2_ASSERT(VALUE, value, VALUE == value);
                        if (veryVerbose) {
                            P(VALUE);
                            P(value);
                        }
                    }

                    {
                        bdlsb::MemOutStreamBuf osb;
                        balber::BerEncoder encoder(&options);
                        ASSERT(0 == encoder.encode(&osb, VALUE));

                        if (veryVerbose) {
                            P(osb.length());
                            printBuffer(osb.data(), osb.length());
                        }

                        bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                      osb.length());
                        ASSERT(0 == decoder.decode(&isb, &value));
                        printDiagnostic(decoder);

                        LOOP2_ASSERT(VALUE, value, VALUE == value);
                        if (veryVerbose) {
                            P(VALUE);
                            P(value);
                        }
                    }

                    {
                        bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                        balber::BerEncoder encoder(&options);
                        ASSERT(0 == encoder.encode(&osb1, VALUE1));
                        ASSERT(0 == encoder.encode(&osb2, VALUE2));
                        ASSERT(0 == encoder.encode(&osb3, VALUE3));

                        if (veryVerbose) {
                            P(osb1.length());
                            P(osb2.length());
                            P(osb3.length());
                            printBuffer(osb1.data(), osb1.length());
                            printBuffer(osb2.data(), osb2.length());
                            printBuffer(osb3.data(), osb3.length());
                        }

                        bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                       osb1.length());
                        bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                       osb2.length());
                        bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                       osb3.length());

                        ASSERT(0 == decoder.decode(&isb1, &value1));
                        ASSERT(0 == decoder.decode(&isb2, &value2));
                        ASSERT(0 == decoder.decode(&isb3, &value3));
                        printDiagnostic(decoder);

                        LOOP2_ASSERT(VALUE1, value1, VALUE1 == value1);
                        LOOP2_ASSERT(VALUE2, value2, VALUE2 == value2);
                        LOOP2_ASSERT(VALUE3, value3, VALUE3 == value3);
                        if (veryVerbose) {
                            P(VALUE1); P(value1);
                            P(VALUE2); P(value2);
                            P(VALUE3); P(value3);
                        }
                    }
                }
            }
            }
            }
        }

        if (verbose) bsl::cout << "\nTesting Time Brute force." << bsl::endl;
        {
            for (int hour = 0; hour <= 23; ++hour) {
                for (int min = 0; min < 60; ++min) {
                    for (int sec = 0; sec < 60; ++sec) {
                        if (veryVerbose) { P_(hour) P_(min) P(sec) }

                        const int MS = 0;
                        const bdlt::Time VALUE(hour, min, sec, MS);
                        bdlt::Time value;
                        const int MS1 = 0, MS2 = 500, MS3 = 999;
                        const int OFF1 = 0, OFF2 =-1439, OFF3 =1439;
                        const bdlt::TimeTz VALUE1(bdlt::Time(hour,
                                                           min,
                                                           sec,
                                                           MS1),
                                                 OFF1);
                        const bdlt::TimeTz VALUE2(bdlt::Time(hour,
                                                           min,
                                                           sec,
                                                           MS2),
                                                 OFF2);
                        const bdlt::TimeTz VALUE3(bdlt::Time(hour,
                                                           min,
                                                           sec,
                                                           MS3),
                                                 OFF3);
                        bdlt::TimeTz value1, value2, value3;

                        {
                            bdlsb::MemOutStreamBuf osb;
                            balber::BerEncoder encoder(&DEFOPTS);
                            ASSERT(0 == encoder.encode(&osb, VALUE));

                            if (veryVerbose) {
                                P(osb.length());
                                printBuffer(osb.data(), osb.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                          osb.length());
                            ASSERT(0 == decoder.decode(&isb, &value));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE, value, VALUE == value);
                            if (veryVerbose) {
                                P(VALUE);
                                P(value);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                            balber::BerEncoder encoder(&DEFOPTS);
                            ASSERT(0 == encoder.encode(&osb1, VALUE1));
                            ASSERT(0 == encoder.encode(&osb2, VALUE2));
                            ASSERT(0 == encoder.encode(&osb3, VALUE3));

                            if (veryVerbose) {
                                P(osb1.length());
                                P(osb2.length());
                                P(osb3.length());
                                printBuffer(osb1.data(), osb1.length());
                                printBuffer(osb2.data(), osb2.length());
                                printBuffer(osb3.data(), osb3.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                           osb1.length());
                            bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                           osb2.length());
                            bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                           osb3.length());

                            ASSERT(0 == decoder.decode(&isb1, &value1));
                            ASSERT(0 == decoder.decode(&isb2, &value2));
                            ASSERT(0 == decoder.decode(&isb3, &value3));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE1, value1, VALUE1 == value1);
                            LOOP2_ASSERT(VALUE2, value2, VALUE2 == value2);
                            LOOP2_ASSERT(VALUE3, value3, VALUE3 == value3);
                            if (veryVerbose) {
                                P(VALUE1); P(value1);
                                P(VALUE2); P(value2);
                                P(VALUE3); P(value3);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb;
                            balber::BerEncoder encoder(&options);
                            ASSERT(0 == encoder.encode(&osb, VALUE));

                            if (veryVerbose) {
                                P(osb.length());
                                printBuffer(osb.data(), osb.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                          osb.length());
                            ASSERT(0 == decoder.decode(&isb, &value));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE, value, VALUE == value);
                            if (veryVerbose) {
                                P(VALUE);
                                P(value);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                            balber::BerEncoder encoder(&options);
                            ASSERT(0 == encoder.encode(&osb1, VALUE1));
                            ASSERT(0 == encoder.encode(&osb2, VALUE2));
                            ASSERT(0 == encoder.encode(&osb3, VALUE3));

                            if (veryVerbose) {
                                P(osb1.length());
                                P(osb2.length());
                                P(osb3.length());
                                printBuffer(osb1.data(), osb1.length());
                                printBuffer(osb2.data(), osb2.length());
                                printBuffer(osb3.data(), osb3.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                           osb1.length());
                            bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                           osb2.length());
                            bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                           osb3.length());

                            ASSERT(0 == decoder.decode(&isb1, &value1));
                            ASSERT(0 == decoder.decode(&isb2, &value2));
                            ASSERT(0 == decoder.decode(&isb3, &value3));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE1, value1, VALUE1 == value1);
                            LOOP2_ASSERT(VALUE2, value2, VALUE2 == value2);
                            LOOP2_ASSERT(VALUE3, value3, VALUE3 == value3);
                            if (veryVerbose) {
                                P(VALUE1); P(value1);
                                P(VALUE2); P(value2);
                                P(VALUE3); P(value3);
                            }
                        }
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting Datetime Brute force."
                               << bsl::endl;
        {
            const int YEARS[] = { 1, 4, 96, 100, 400, 500, 800, 1000, 1600,
                                  1700, 1751, 1752, 1753, 1930, 2000, 2010,
                                  2012, 2019, 2020, 2021, 6478, 6479, 6480,
                                  9998, 9999 };
            const int NUM_YEARS = sizeof YEARS / sizeof *YEARS;

            const int MONTHS[] = { 1, 2, 5, 8, 9, 12 };
            const int NUM_MONTHS = sizeof MONTHS / sizeof *MONTHS;

            const int DAYS[] = { 1, 2, 5, 10, 15, 20, 28, 29, 30, 31 };
            const int NUM_DAYS = sizeof DAYS / sizeof *DAYS;

            for (int di = 0; di < NUM_YEARS; ++di) {
            for (int dj = 0; dj < NUM_MONTHS; ++dj) {
            for (int dk = 0; dk < NUM_DAYS; ++dk) {

                const int YEAR  = YEARS[di];
                const int MONTH = MONTHS[dj];
                const int DAY   = DAYS[dk];

                if (bdlt::Date::isValidYearMonthDay(YEAR, MONTH, DAY)
                 && bdlt::ProlepticDateImpUtil::isValidYearMonthDay(YEAR,
                                                                    MONTH,
                                                                    DAY)) {

                    const int HOURS[] = { 0, 12, 23 };
                    const int NUM_HOURS = sizeof HOURS / sizeof *HOURS;

                    const int MINS[] = { 0, 30, 59 };
                    const int NUM_MINS = sizeof MINS / sizeof *MINS;

                    const int SECONDS[] = { 0, 30, 59 };
                    const int NUM_SECS = sizeof SECONDS / sizeof *SECONDS;

                    for (int ti = 0; ti < NUM_HOURS; ++ti) {
                    for (int tj = 0; tj < NUM_MINS; ++tj) {
                    for (int tk = 0; tk < NUM_SECS; ++tk) {

                        const int HOUR = HOURS[ti];
                        const int MIN  = MINS[tj];
                        const int SECS = SECONDS[tk];

                        if (veryVerbose) { P_(YEAR) P_(MONTH) P(DAY) }
                        if (veryVerbose) { P_(HOUR) P_(MIN) P(SECS) }

                        const int MS = 0;
                        const bdlt::Date DATE(YEAR, MONTH, DAY);
                        const bdlt::Time TIME(HOUR, MIN, SECS, MS);
                        const bdlt::Datetime VALUE(DATE, TIME);
                        bdlt::Datetime value;
                        const int MS1 = 0, MS2 = 500, MS3 = 999;
                        const int OFF1 = 0, OFF2 =-1439, OFF3 =1439;
                        const bdlt::Date DATE1(YEAR, MONTH, DAY);
                        const bdlt::Time TIME1(HOUR, MIN, SECS, MS1);

                        const bdlt::Date DATE2(YEAR, MONTH, DAY);
                        const bdlt::Time TIME2(HOUR, MIN, SECS, MS2);

                        const bdlt::Date DATE3(YEAR, MONTH, DAY);
                        const bdlt::Time TIME3(HOUR, MIN, SECS, MS3);

                        const bdlt::Datetime DT1(DATE1, TIME1);
                        const bdlt::Datetime DT2(DATE2, TIME2);
                        const bdlt::Datetime DT3(DATE3, TIME3);

                        const bdlt::DatetimeTz VALUE1(DT1, OFF1);
                        const bdlt::DatetimeTz VALUE2(DT2, OFF2);
                        const bdlt::DatetimeTz VALUE3(DT3, OFF3);

                        bdlt::DatetimeTz value1, value2, value3;

                        test::BasicRecord VALUE4;
                        VALUE4.i1() = 100;
                        VALUE4.i2() = 200;
                        VALUE4.dt() = VALUE1;
                        VALUE4.s()  = "Hello World";

                        test::BasicRecord VALUE5(VALUE4), VALUE6(VALUE4);
                        VALUE5.dt() = VALUE2;
                        VALUE6.dt() = VALUE3;

                        test::BasicRecord value4, value5, value6;

                        {
                            bdlsb::MemOutStreamBuf osb;
                            balber::BerEncoder encoder(&DEFOPTS);
                            ASSERT(0 == encoder.encode(&osb, VALUE));

                            if (veryVerbose) {
                                P(osb.length());
                                printBuffer(osb.data(), osb.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                          osb.length());
                            ASSERT(0 == decoder.decode(&isb, &value));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE, value, VALUE == value);
                            if (veryVerbose) {
                                P(VALUE);
                                P(value);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                            balber::BerEncoder encoder(&DEFOPTS);
                            ASSERT(0 == encoder.encode(&osb1, VALUE1));
                            ASSERT(0 == encoder.encode(&osb2, VALUE2));
                            ASSERT(0 == encoder.encode(&osb3, VALUE3));

                            if (veryVerbose) {
                                P(osb1.length());
                                P(osb2.length());
                                P(osb3.length());
                                printBuffer(osb1.data(), osb1.length());
                                printBuffer(osb2.data(), osb2.length());
                                printBuffer(osb3.data(), osb3.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                           osb1.length());
                            bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                           osb2.length());
                            bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                           osb3.length());

                            ASSERT(0 == decoder.decode(&isb1, &value1));
                            ASSERT(0 == decoder.decode(&isb2, &value2));
                            ASSERT(0 == decoder.decode(&isb3, &value3));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE1, value1, VALUE1 == value1);
                            LOOP2_ASSERT(VALUE2, value2, VALUE2 == value2);
                            LOOP2_ASSERT(VALUE3, value3, VALUE3 == value3);
                            if (veryVerbose) {
                                P(VALUE1); P(value1);
                                P(VALUE2); P(value2);
                                P(VALUE3); P(value3);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                            balber::BerEncoder encoder(&DEFOPTS);
                            ASSERT(0 == encoder.encode(&osb1, VALUE4));
                            ASSERT(0 == encoder.encode(&osb2, VALUE5));
                            ASSERT(0 == encoder.encode(&osb3, VALUE6));

                            if (veryVerbose) {
                                P(osb1.length());
                                P(osb2.length());
                                P(osb3.length());
                                printBuffer(osb1.data(), osb1.length());
                                printBuffer(osb2.data(), osb2.length());
                                printBuffer(osb3.data(), osb3.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                           osb1.length());
                            bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                           osb2.length());
                            bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                           osb3.length());

                            ASSERT(0 == decoder.decode(&isb1, &value4));
                            ASSERT(0 == decoder.decode(&isb2, &value5));
                            ASSERT(0 == decoder.decode(&isb3, &value6));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE4, value4, VALUE4 == value4);
                            LOOP2_ASSERT(VALUE5, value5, VALUE5 == value5);
                            LOOP2_ASSERT(VALUE6, value6, VALUE6 == value6);
                            if (veryVerbose) {
                                P(VALUE4); P(value4);
                                P(VALUE5); P(value5);
                                P(VALUE6); P(value6);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb;
                            balber::BerEncoder encoder(&options);
                            ASSERT(0 == encoder.encode(&osb, VALUE));

                            if (veryVerbose) {
                                P(osb.length());
                                printBuffer(osb.data(), osb.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb(osb.data(),
                                                          osb.length());
                            ASSERT(0 == decoder.decode(&isb, &value));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE, value, VALUE == value);
                            if (veryVerbose) {
                                P(VALUE);
                                P(value);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                            balber::BerEncoder encoder(&options);
                            ASSERT(0 == encoder.encode(&osb1, VALUE1));
                            ASSERT(0 == encoder.encode(&osb2, VALUE2));
                            ASSERT(0 == encoder.encode(&osb3, VALUE3));

                            if (veryVerbose) {
                                P(osb1.length());
                                P(osb2.length());
                                P(osb3.length());
                                printBuffer(osb1.data(), osb1.length());
                                printBuffer(osb2.data(), osb2.length());
                                printBuffer(osb3.data(), osb3.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                           osb1.length());
                            bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                           osb2.length());
                            bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                           osb3.length());

                            ASSERT(0 == decoder.decode(&isb1, &value1));
                            ASSERT(0 == decoder.decode(&isb2, &value2));
                            ASSERT(0 == decoder.decode(&isb3, &value3));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE1, value1, VALUE1 == value1);
                            LOOP2_ASSERT(VALUE2, value2, VALUE2 == value2);
                            LOOP2_ASSERT(VALUE3, value3, VALUE3 == value3);
                            if (veryVerbose) {
                                P(VALUE1); P(value1);
                                P(VALUE2); P(value2);
                                P(VALUE3); P(value3);
                            }
                        }

                        {
                            bdlsb::MemOutStreamBuf osb1, osb2, osb3;
                            balber::BerEncoder encoder(&options);
                            ASSERT(0 == encoder.encode(&osb1, VALUE4));
                            ASSERT(0 == encoder.encode(&osb2, VALUE5));
                            ASSERT(0 == encoder.encode(&osb3, VALUE6));

                            if (veryVerbose) {
                                P(osb1.length());
                                P(osb2.length());
                                P(osb3.length());
                                printBuffer(osb1.data(), osb1.length());
                                printBuffer(osb2.data(), osb2.length());
                                printBuffer(osb3.data(), osb3.length());
                            }

                            bdlsb::FixedMemInStreamBuf isb1(osb1.data(),
                                                           osb1.length());
                            bdlsb::FixedMemInStreamBuf isb2(osb2.data(),
                                                           osb2.length());
                            bdlsb::FixedMemInStreamBuf isb3(osb3.data(),
                                                           osb3.length());

                            ASSERT(0 == decoder.decode(&isb1, &value4));
                            ASSERT(0 == decoder.decode(&isb2, &value5));
                            ASSERT(0 == decoder.decode(&isb3, &value6));
                            printDiagnostic(decoder);

                            LOOP2_ASSERT(VALUE4, value4, VALUE4 == value4);
                            LOOP2_ASSERT(VALUE5, value5, VALUE5 == value5);
                            LOOP2_ASSERT(VALUE6, value6, VALUE6 == value6);
                            if (veryVerbose) {
                                P(VALUE4); P(value4);
                                P(VALUE5); P(value5);
                                P(VALUE6); P(value6);
                            }
                        }
                    }
                    }
                    }
                }
            }
            }
            }
        }
      } break;
      case 15: {
        // --------------------------------------------------------------------
        // TESTING encoding & decoding for date/time components
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTESTING encoding & decoding for date/time"
                               << "\n========================================="
                               << bsl::endl;

        balber::BerEncoderOptions options;
        options.setEncodeDateAndTimeTypesAsBinary(true);
        const balber::BerEncoderOptions DEFOPTS;

        if (verbose) bsl::cout << "\nDefine data" << bsl::endl;

        static const struct {
            int d_lineNum;   // source line number
            int d_year;      // year under test
            int d_month;     // month under test
            int d_day;       // day under test
            int d_hour;      // hour under test
            int d_minutes;   // minutes under test
            int d_seconds;   // seconds under test
            int d_milliSecs; // milli seconds under test
            int d_tzoffset;  // time zone offset
        } DATA[] = {
   //line no.  year   month   day   hour    min   sec    ms  offset
   //-------   -----  -----   ---   ----    ---   ---    --  ------
    {      L_,      1,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,    1,     0,     0,    0,    0,     45     },
    {      L_,      1,     1,    1,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     1,    1,     1,     1,    1,    1,      0     },
    {      L_,      1,     1,    1,     1,     1,    1,    1,    500     },
    {      L_,      1,     1,    1,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     1,    1,     1,    23,   59,   59,      0     },
    {      L_,      1,     1,    1,     1,    23,   59,   59,   1439     },
    {      L_,      1,     1,    1,     1,    23,   59,   59,  -1439     },

    {      L_,      1,     1,    2,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,    2,     0,     0,    0,    0,   1439     },
    {      L_,      1,     1,    2,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     1,    2,     1,     1,    1,    1,      0     },
    {      L_,      1,     1,    2,     1,     1,    1,    1,    500     },

    {      L_,      1,     1,    2,     1,    23,   59,   59,      0     },
    {      L_,      1,     1,    2,     1,    23,   59,   59,    500     },
    {      L_,      1,     1,    2,     1,    23,   59,   59,   -500     },

    {      L_,      1,     1,   10,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,   10,     1,     1,    1,    1,     99     },

    {      L_,      1,     1,   30,     0,     0,    0,    0,      0     },
    {      L_,      1,     1,   31,     0,     0,    0,    0,   1439     },
    {      L_,      1,     1,   31,     0,     0,    0,    0,  -1439     },

    {      L_,      1,     2,    1,     0,     0,    0,    0,      0     },
    {      L_,      1,     2,    1,    23,    59,   59,    0,   1439     },

    {      L_,      1,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,      1,    12,   31,    23,    59,   59,    0,   1439     },

    {      L_,      2,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,      2,     1,    1,    23,    59,   59,    0,   1439     },

    {      L_,      4,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,      4,     1,    1,    23,    59,   59,    0,   1439     },

    {      L_,      4,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,      4,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,      4,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,      4,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,      4,     2,   29,    23,    59,   59,    0,   1439     },
    {      L_,      4,     2,   29,    23,    59,   59,    0,  -1439     },

    {      L_,      4,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,      4,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,      4,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,      8,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,      8,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,      8,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,      8,     2,   29,    23,    59,   59,    0,   1439     },

    {      L_,      8,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,      8,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,    100,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    100,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,    100,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,    100,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    100,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,    100,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,    400,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    400,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,    400,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,    400,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,    400,     2,   29,    23,    59,   59,    0,   1439     },
    {      L_,    400,     2,   29,    23,    59,   59,    0,  -1439     },

    {      L_,    400,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    400,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,    400,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,    500,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    500,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,    500,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    500,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,    800,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,    800,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,    800,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,    800,     2,   29,    23,    59,   59,    0,   1439     },

    {      L_,    800,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,    800,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,   1000,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   1000,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,   1000,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   1000,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,   2000,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   2000,     2,   28,    23,    59,   59,    0,   1439     },

    {      L_,   2000,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,   2000,     2,   29,    23,    59,   59,    0,   1439     },

    {      L_,   2000,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   2000,     3,    1,    23,    59,   59,    0,   1439     },

    {      L_,   2016,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   2017,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   2018,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   2019,    12,   31,     0,     0,    0,    0,      0     },

    {      L_,   2020,     1,    1,     0,     0,    0,    0,      0     },
    {      L_,   2020,     1,    1,     0,     0,    0,    0,   1439     },
    {      L_,   2020,     1,    1,     0,     0,    0,    0,  -1439     },

    {      L_,   2020,     1,    1,    23,    59,   59,  999,      0     },
    {      L_,   2020,     1,    1,    23,    59,   59,  999,   1439     },
    {      L_,   2020,     1,    1,    23,    59,   59,  999,  -1439     },

    {      L_,   2020,     1,    2,     0,     0,    0,    0,      0     },
    {      L_,   2020,     1,    2,     0,     0,    0,    0,   1439     },
    {      L_,   2020,     1,    2,     0,     0,    0,    0,  -1439     },

    {      L_,   2020,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   2020,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,   2020,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,   2020,     2,   29,     0,     0,    0,    0,      0     },
    {      L_,   2020,     2,   29,    23,    59,   59,    0,   1439     },
    {      L_,   2020,     2,   29,    23,    59,   59,    0,  -1439     },

    {      L_,   2020,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   2020,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,   2020,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,   2021,     1,    2,     0,     0,    0,    0,      0     },
    {      L_,   2022,     1,    2,     0,     0,    0,    0,      0     },

    {      L_,   9999,     2,   28,     0,     0,    0,    0,      0     },
    {      L_,   9999,     2,   28,    23,    59,   59,    0,   1439     },
    {      L_,   9999,     2,   28,    23,    59,   59,    0,  -1439     },

    {      L_,   9999,     3,    1,     0,     0,    0,    0,      0     },
    {      L_,   9999,     3,    1,    23,    59,   59,    0,   1439     },
    {      L_,   9999,     3,    1,    23,    59,   59,    0,  -1439     },

    {      L_,   9999,    12,   30,     0,     0,    0,    0,      0     },
    {      L_,   9999,    12,   30,    23,    59,   59,    0,   1439     },

    {      L_,   9999,    12,   31,     0,     0,    0,    0,      0     },
    {      L_,   9999,    12,   31,    23,    59,   59,    0,   1439     },
        };
        const int NUM_DATA = sizeof DATA / sizeof *DATA;

        if (verbose) bsl::cout << "\nTesting 'bdlt::Date'." << bsl::endl;
        {
            typedef bdlt::Date Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;

                if (veryVerbose) { P_(Y) P_(M) P(D) }

                const Type VALUE(Y, M, D); Type value;

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::DateTz'." << bsl::endl;
        {
            typedef bdlt::DateTz Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;
                const int OFF  = DATA[i].d_tzoffset;

                const Type VALUE(bdlt::Date(Y, M, D), OFF); Type value;

                if (veryVerbose) { P_(Y) P_(M) P_(D) P(OFF) }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::Time'." << bsl::endl;
        {
            typedef bdlt::Time Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;

                if (veryVerbose) { P_(H) P_(MM) P_(S) P(MS) }

                const Type VALUE(H, MM, S, MS); Type value;
                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::TimeTz'." << bsl::endl;
        {
            typedef bdlt::TimeTz Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;
                const int OFF  = DATA[i].d_tzoffset;

                const Type VALUE(bdlt::Time(H, MM, S, MS), OFF); Type value;

                if (veryVerbose) { P_(H) P_(MM) P_(S) P_(MS) P(OFF) }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::Datetime'." << bsl::endl;
        {
            typedef bdlt::Datetime Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;

                const Type VALUE(Y, M, D, H, MM, S, MS); Type value;

                if (veryVerbose) { P_(Y) P_(M) P_(D) P_(H)
                                   P_(MM) P_(S) P(MS) }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }
            }
        }

        if (verbose) bsl::cout << "\nTesting 'bdlt::DatetimeTz'." << bsl::endl;
        {
            typedef bdlt::DatetimeTz Type;

            for (int i = 0; i < NUM_DATA ; ++i) {
                const int LINE = DATA[i].d_lineNum;
                const int Y    = DATA[i].d_year;
                const int M    = DATA[i].d_month;
                const int D    = DATA[i].d_day;
                const int H    = DATA[i].d_hour;
                const int MM   = DATA[i].d_minutes;
                const int S    = DATA[i].d_seconds;
                const int MS   = DATA[i].d_milliSecs;
                const int OFF  = DATA[i].d_tzoffset;

                const Type VALUE(bdlt::Datetime(Y, M, D, H, MM, S, MS), OFF);
                Type value;

                if (veryVerbose) { P_(Y) P_(M) P_(D) P_(H)
                                   P_(MM) P_(S) P_(MS) P(OFF) }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&options);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }

                {
                    bdlsb::MemOutStreamBuf osb;
                    balber::BerEncoder encoder(&DEFOPTS);
                    ASSERT(0 == encoder.encode(&osb, VALUE));

                    if (veryVerbose) {
                        P(osb.length());
                        printBuffer(osb.data(), osb.length());
                    }

                    bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                    ASSERT(0 == decoder.decode(&isb, &value));
                    printDiagnostic(decoder);

                    LOOP3_ASSERT(LINE, VALUE, value, VALUE == value);
                    if (veryVerbose) {
                        P(VALUE);
                        P(value);
                    }
                }
            }
        }
      } break;
      case 14: {
        // --------------------------------------------------------------------
        // TESTING functions related to skipped elements
        //   This test exercises functions that apply to skipped elements.
        //
        // Concerns:
        //   a. The setNumUnknownElementsSkipped sets the number of skipped
        //      elements correctly.
        //   b. The numUnknownElementsSkipped returns the number of skipped
        //      elements correctly.
        //
        // Plan:
        //
        // Testing:
        //   void setNumUnknownElementsSKipped(int value);
        //   int numUnknownElementsSkipped() const;
        // --------------------------------------------------------------------

        if (verbose) cout << "\nFUNCTIONS related to skipped elements"
                          << "\n=====================================" << endl;

        if (verbose) cout << "\nTesting setting and getting num skipped elems."
                          << endl;
        {
            balber::BerDecoder mX; const balber::BerDecoder& X = mX;
            ASSERT(0 == X.numUnknownElementsSkipped());

            const int DATA[] = { 0, 1, 5, 100, 2000 };
            const int NUM_DATA = sizeof DATA / sizeof *DATA;
            for (int i = 0; i < NUM_DATA; ++i) {
                const int NUM_SKIPPED_ELEMS = DATA[i];
                mX.setNumUnknownElementsSkipped(NUM_SKIPPED_ELEMS);
                LOOP3_ASSERT(i, NUM_SKIPPED_ELEMS,
                           X.numUnknownElementsSkipped(),
                           NUM_SKIPPED_ELEMS == X.numUnknownElementsSkipped());
            }
        }
      } break;
      case 13: {
        // --------------------------------------------------------------------
        // TESTING SEQUENCE DECODED AS CHOICE
        //
        // Concerns:
        //   Decoder needs to be able to handle decoding a sequence as a
        //   choice without aborting, should simply log error messages.
        // --------------------------------------------------------------------

        {
            if (verbose) bsl::cout << "\nTesting Sequence Decoded as Choice\n"
                                        "==================================\n";

            bdlsb::MemOutStreamBuf osb;

            test::MySequence valueOut;
            valueOut.attribute1() = 34;
            valueOut.attribute2() = "Hello";

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MyChoice valueIn;

            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 != decoder.decode(&isb, &valueIn));
            ASSERT(decoder.loggedMessages().length() > 0);
            if (veryVerbose) {
                bsl::cout << "We expect the decoder to complain.  Here is\n"
                             "what it says:\n";
                printDiagnostic(decoder);
            }
        }

        {
            if (verbose) bsl::cout <<
                                "\nTesting Empty Sequence Decoded as Choice\n"
                                  "========================================\n";

            bdlsb::MemOutStreamBuf osb;

            test::MySequence valueOut;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MyChoice valueIn;

            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 != decoder.decode(&isb, &valueIn));
            ASSERT(decoder.loggedMessages().length() > 0);
            if (veryVerbose) {
                bsl::cout << "We expect the decoder to complain.  Here is\n"
                             "what it says:\n";
                printDiagnostic(decoder);
            }
        }
      } break;
      case 12: {
        // --------------------------------------------------------------------
        // TESTING CHOICE DECODED AS SEQUENCE
        //
        // Concerns:
        //   Decoder needs to be able to handle decoding a choice as a
        //   sequence without aborting, should simply log error messages.
        // --------------------------------------------------------------------

        {
            if (verbose) bsl::cout << "\nTesting Choice Decoded as Sequence\n"
                                        "==================================\n";

            bdlsb::MemOutStreamBuf osb;

            test::MyChoice valueOut;
            valueOut.makeSelection1();
            valueOut.selection1() = 34;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequence valueIn;

            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 != decoder.decode(&isb, &valueIn));
            ASSERT(decoder.loggedMessages().length() > 0);
            if (veryVerbose) {
                bsl::cout << "We expect the decoder to complain.  Here is\n"
                             "what it says:\n";
                printDiagnostic(decoder);
            }
        }

        {
            if (verbose) bsl::cout <<
                                 "\nTesting Null Choice Decoded as Sequence\n"
                                   "=======================================\n";

            bdlsb::MemOutStreamBuf osb;

            test::MyChoice valueOut;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequence valueIn;

            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 != decoder.decode(&isb, &valueIn));
            ASSERT(decoder.loggedMessages().length() > 0);
            if (veryVerbose) {
                bsl::cout << "We expect the decoder to complain.  Here is\n"
                             "what it says:\n";
                printDiagnostic(decoder);
            }
        }
      } break;
      case 11: {
        // --------------------------------------------------------------------
        // TESTING NILLABLE VALUES
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Nillable Values"
                               << "\n=======================" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting with null value." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithNillable valueOut;
            valueOut.attribute1() = 34;
            valueOut.attribute2() = "Hello";

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithNillable valueIn;
            valueIn.myNillable() = "Hello";

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "300C8001 22A10082 0548656C 6C6F";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNillable valueFromNokalva;
                valueFromNokalva.myNillable() = "Hello";

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22A18000 00820548 "
                                            "656C6C6F 0000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNillable valueFromNokalva;
                valueFromNokalva.myNillable() = "Hello";

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nTesting with non-null value."
                               << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithNillable valueOut;
            valueOut.attribute1() = 34;
            valueOut.myNillable() = "World!";
            valueOut.attribute2() = "Hello";

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithNillable valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30148001 22A10880 06576F72 "
                                            "6C642182 0548656C 6C6F";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNillable valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22A18080 06576F72 "
                                            "6C642100 00820548 656C6C6F 0000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNillable valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 10: {
        // --------------------------------------------------------------------
        // TESTING ANONYMOUS CHOICES
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Anonymous Choices"
                               << "\n=========================" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting with no selection." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithAnonymousChoice valueOut;
            valueOut.attribute1() = 34;
            valueOut.attribute2() = "Hello";

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithAnonymousChoice valueIn;
            valueIn.choice().makeMyChoice2("Hello");

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting with selection." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithAnonymousChoice valueOut;
            valueOut.attribute1() = 34;
            valueOut.choice().makeMyChoice1(65);
            valueOut.attribute2() = "Hello";

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithAnonymousChoice valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "300F8001 22A10380 01418205 "
                                            "48656C6C 6F";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithAnonymousChoice valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22A18080 01410000 "
                                            "82054865 6C6C6F00 00";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithAnonymousChoice valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithAnonymousChoice valueOut;
            valueOut.attribute1() = 34;
            valueOut.choice().makeMyChoice2("World!");
            valueOut.attribute2() = "Hello";

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithAnonymousChoice valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30148001 22A10881 06576F72 "
                                            "6C642182 0548656C 6C6F";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithAnonymousChoice valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22A18081 06576F72 "
                                            "6C642100 00820548 656C6C6F 0000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithAnonymousChoice valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 9: {
        // --------------------------------------------------------------------
        // TESTING ARRAYS
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Arrays"
                               << "\n==============" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting with empty array." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithArray valueOut;
            valueOut.attribute1() = 34;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithArray valueIn;
            valueIn.attribute2().push_back("This");
            valueIn.attribute2().push_back("is");
            valueIn.attribute2().push_back("a");
            valueIn.attribute2().push_back("test.");

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30058001 22A100";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithArray valueFromNokalva;
                valueFromNokalva.attribute2().push_back("This");
                valueFromNokalva.attribute2().push_back("is");
                valueFromNokalva.attribute2().push_back("a");
                valueFromNokalva.attribute2().push_back("test.");

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22A18000 000000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithArray valueFromNokalva;
                valueFromNokalva.attribute2().push_back("This");
                valueFromNokalva.attribute2().push_back("is");
                valueFromNokalva.attribute2().push_back("a");
                valueFromNokalva.attribute2().push_back("test.");

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nTesting with empty array and"
                               << " 'encodeEmptyArrays' option set to 'false'"
                               << bsl::endl;
        {
            balber::BerEncoderOptions options;
            options.setEncodeEmptyArrays(false);

            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithArray valueOut;
            valueOut.attribute1() = 34;

            balber::BerEncoder encoder(&options);
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithArray valueIn;
            valueIn.attribute2().push_back("This");
            valueIn.attribute2().push_back("is");
            valueIn.attribute2().push_back("a");
            valueIn.attribute2().push_back("test.");

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting with non-empty array."
                               << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithArray valueOut;
            valueOut.attribute1() = 34;
            valueOut.attribute2().push_back("Hello");
            valueOut.attribute2().push_back("World!");

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithArray valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30148001 22A10F0C 0548656C "
                                            "6C6F0C06 576F726C 6421";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithArray valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22A1800C 0548656C "
                                            "6C6F0C06 576F726C 64210000 0000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithArray valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 8: {
        // --------------------------------------------------------------------
        // TESTING NULLABLE VALUES
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Nullable Values"
                               << "\n=======================" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting with null value." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithNullable valueOut;
            valueOut.attribute1() = 34;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithNullable valueIn;
            valueIn.attribute1() = 356;
            valueIn.attribute2().makeValue("Hello");

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30038001 22";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNullable valueFromNokalva;
                valueFromNokalva.attribute1() = 356;
                valueFromNokalva.attribute2().makeValue("Hello");

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 220000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNullable valueFromNokalva;
                valueFromNokalva.attribute1() = 356;
                valueFromNokalva.attribute2().makeValue("Hello");

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nTesting with non-null value."
                               << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MySequenceWithNullable valueOut;
            valueOut.attribute1() = 34;
            valueOut.attribute2().makeValue("Hello");

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MySequenceWithNullable valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "300A8001 22810548 656C6C6F";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNullable valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "30808001 22810548 656C6C6F 0000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MySequenceWithNullable valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 7: {
        // --------------------------------------------------------------------
        // TESTING CHOICES
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Choices"
                               << "\n===============" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting with no selection." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MyChoice valueOut;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MyChoice valueIn;
            valueIn.makeSelection2();
            valueIn.selection2() = "Hello";

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting with selection." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MyChoice valueOut;
            valueOut.makeSelection1();
            valueOut.selection1() = 34;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MyChoice valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(DEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "3005A003 800122";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MyChoice valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva "
                                          "(INDEFINITE LENGTH)."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "3080A080 80012200 000000";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MyChoice valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 6: {
        // --------------------------------------------------------------------
        // TESTING SEQUENCES
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Sequences"
                               << "\n=================" << bsl::endl;

        bdlsb::MemOutStreamBuf osb;

        test::MySequence valueOut;
        valueOut.attribute1() = 34;
        valueOut.attribute2() = "Hello";

        ASSERT(0 == encoder.encode(&osb, valueOut));

        if (veryVerbose) {
            P(osb.length())
            printBuffer(osb.data(), osb.length());
        }

        test::MySequence valueIn;

        ASSERT(valueOut != valueIn);
        bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
        ASSERT(0 == decoder.decode(&isb, &valueIn));
        printDiagnostic(decoder);

        ASSERT(valueOut == valueIn);

        static const struct {
            int         d_line;
            const char *d_nokalvaData;
            int         d_numUnknownElements;
        } DATA[] = {
            //Line Nokalva Data                                     Num Unknown
            //==== ============                                     ===========

            // Definite length:
            { L_,  "300A 800122         810548656C6C6F"                  , 0 },
            { L_,  "300A 810548656C6C6F 800122"                          , 0 },

            { L_,  "300D 820199         800122         810548656C6C6F"   , 1 },
            { L_,  "300D 820199         810548656C6C6F 800122"           , 1 },
            { L_,  "300F A203820199     810548656C6C6F 800122"           , 1 },
            { L_,  "3011 A2808201990000 810548656C6C6F 800122"           , 1 },
            //           ^^^^^^: Extra (unknown) attr3

            { L_,  "300D 800122         820199         810548656C6C6F"   , 1 },
            { L_,  "300D 810548656C6C6F 820199         800122"           , 1 },
            { L_,  "300F 810548656C6C6F A203820199     800122"           , 1 },
            { L_,  "3011 810548656C6C6F A2808201990000 800122"           , 1 },
            //   Extra (unknown) attr3: ^^^^^^

            { L_,  "300D 800122         810548656C6C6F 820199"           , 1 },
            { L_,  "300D 810548656C6C6F 800122         820199"           , 1 },
            { L_,  "300F 810548656C6C6F 800122         A203820199"       , 1 },
            { L_,  "3011 810548656C6C6F 800122         A2808201990000"   , 1 },
            //                  Extra (unknown) attr3: ^^^^^^

            // Indefinite length:
            { L_,  "3080 800122         810548656C6C6F 0000"             , 0 },
            { L_,  "3080 810548656C6C6F 800122         0000"             , 0 },

            { L_,  "3080 820199         800122         810548656C6C6F 0000",1},
            { L_,  "3080 820199         810548656C6C6F 800122         0000",1},
            { L_,  "3080 A203820199     810548656C6C6F 800122         0000",1},
            { L_,  "3080 A2808201990000 810548656C6C6F 800122         0000",1},
            //           ^^^^^^: Extra (unknown) attr3

            { L_,  "3080 800122         820199         810548656C6C6F 0000",1},
            { L_,  "3080 810548656C6C6F 820199         800122         0000",1},
            { L_,  "3080 810548656C6C6F A203820199     800122         0000",1},
            { L_,  "3080 810548656C6C6F A2808201990000 800122         0000",1},
            //   Extra (unknown) attr3: ^^^^^^

            { L_,  "3080 800122         810548656C6C6F 820199         0000",1},
            { L_,  "3080 810548656C6C6F 800122         820199         0000",1},
            { L_,  "3080 810548656C6C6F 800122         A203820199     0000",1},
            { L_,  "3080 810548656C6C6F 800122         A2808201990000 0000",1},
            //                  Extra (unknown) attr3: ^^^^^^
        };

        static const int DATA_LEN = sizeof(DATA) / sizeof(DATA[0]);

        if (verbose) bsl::cout << "Testing with skipUnknownElements == true"
                               << bsl::endl;
        for (int i = 0; i < DATA_LEN; ++i)
        {
            const int   LINE         = DATA[i].d_line;
            const char* NOKALVA_DATA = DATA[i].d_nokalvaData;
            const int   NUM_UNKNOWN  = DATA[i].d_numUnknownElements;

            bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);
            const char *berData = &nokalvaData[0];
            const bsl::size_t berDataLen = nokalvaData.size();

            test::MySequence valueFromNokalva;

            ASSERT(valueOut != valueFromNokalva);
            bdlsb::FixedMemInStreamBuf isb(berData, berDataLen);
            decoder.setNumUnknownElementsSkipped(0);
            int ret = decoder.decode(&isb, &valueFromNokalva);
            printDiagnostic(decoder);
            LOOP3_ASSERT(LINE,
                         NUM_UNKNOWN,
                         decoder.numUnknownElementsSkipped(),
                         NUM_UNKNOWN == decoder.numUnknownElementsSkipped());

            LOOP_ASSERT(LINE, 0 == ret);
            LOOP_ASSERT(LINE, berDataLen == static_cast<bsl::size_t>(isb.pubseekoff(0, bsl::ios::cur)));
            LOOP_ASSERT(LINE, valueOut == valueFromNokalva);
        }

        if (verbose) bsl::cout << "Testing with skipUnknownElements == false"
                               << bsl::endl;
        for (int i = 0; i < DATA_LEN; ++i)
        {
            const int   LINE         = DATA[i].d_line;
            const char* NOKALVA_DATA = DATA[i].d_nokalvaData;
            const int   NUM_UNKNOWN  = DATA[i].d_numUnknownElements;

            bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);
            const char *berData = &nokalvaData[0];
            const bsl::size_t berDataLen = nokalvaData.size();

            test::MySequence valueFromNokalva;

            ASSERT(valueOut != valueFromNokalva);
            bdlsb::FixedMemInStreamBuf isb(berData, berDataLen);

            balber::BerDecoderOptions opts;
            opts.setSkipUnknownElements(false);
            if (veryVeryVerbose) {
               opts.setTraceLevel(1);
            }

            balber::BerDecoder decoder2(&opts);
            int ret = decoder2.decode(&isb, &valueFromNokalva);
            printDiagnostic(decoder2);

            if (NUM_UNKNOWN) {
                LOOP_ASSERT(LINE, 0 != ret);
                LOOP_ASSERT(LINE,
                          NUM_UNKNOWN == decoder2.numUnknownElementsSkipped());
            }
            else {
                LOOP_ASSERT(LINE,
                            berDataLen == static_cast<bsl::size_t>(isb.pubseekoff(0, bsl::ios::cur)));
                LOOP_ASSERT(LINE, valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;

      } break;
      case 5: {
        // --------------------------------------------------------------------
        // TESTING ENUMERATIONS
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Enumerations"
                               << "\n====================" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting with VALUE1." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MyEnumeration::Value valueOut = test::MyEnumeration::VALUE1;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MyEnumeration::Value valueIn = test::MyEnumeration::VALUE2;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "0A0100";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MyEnumeration::Value valueFromNokalva
                                                 = test::MyEnumeration::VALUE2;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nTesting with VALUE2." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::MyEnumeration::Value valueOut = test::MyEnumeration::VALUE2;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::MyEnumeration::Value valueIn = test::MyEnumeration::VALUE1;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "0A0101";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::MyEnumeration::Value valueFromNokalva
                                                 = test::MyEnumeration::VALUE1;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 4: {
        // --------------------------------------------------------------------
        // TESTING CUSTOMIZED TYPES
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nTesting Customized Types"
                               << "\n========================" << bsl::endl;

        const bsl::string VALUE = "Hello";

        if (verbose) bsl::cout << "\nEncoding customized string." << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            test::CustomizedString valueOut;
            valueOut.fromString(VALUE);

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::CustomizedString valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);

            if (veryVerbose) bsl::cout << "\tDecoding from Nokalva."
                                       << bsl::endl;
            {
                const char NOKALVA_DATA[] = "0C054865 6C6C6F";

                bsl::vector<char> nokalvaData = loadFromHex(NOKALVA_DATA);

                test::CustomizedString valueFromNokalva;

                ASSERT(valueOut != valueFromNokalva);
                bdlsb::FixedMemInStreamBuf isb(&nokalvaData[0],
                                              nokalvaData.size());
                ASSERT(0 == decoder.decode(&isb, &valueFromNokalva));
                printDiagnostic(decoder);

                ASSERT(valueOut == valueFromNokalva);
            }
        }

        if (verbose) bsl::cout << "\nEncoding bsl::string (control)."
                               << bsl::endl;
        {
            bdlsb::MemOutStreamBuf osb;

            bsl::string valueOut = VALUE;

            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            test::CustomizedString valueIn;

            ASSERT(valueOut != valueIn.toString());
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn.toString());
        }

        if (verbose) bsl::cout << "\nEnd of test." << bsl::endl;
      } break;
      case 3: {
        // --------------------------------------------------------------------
        // VOCABULARY TYPES TEST
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nVOCABULARY TYPES TEST"
                               << "\n=====================" << bsl::endl;

        if (verbose) bsl::cout << "\nTesting bdlt::Date"
                               << "\n=================" << bsl::endl;

        {
            const int YEAR = 2005, MONTH = 12, DAY = 15;

            bdlt::Date valueOut(YEAR, MONTH, DAY);

            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            bdlt::Date valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting bdlt::DateTz"
                               << "\n===================" << bsl::endl;

        {
            const int YEAR = 2005, MONTH = 12, DAY = 15, OFFSET = 45;

            bdlt::DateTz valueOut(bdlt::Date(YEAR, MONTH, DAY), OFFSET);

            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            bdlt::DateTz valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting bdlt::Time"
                               << "\n=================" << bsl::endl;

        {
            const int HOUR = 12, MIN = 56, SECS = 9, MILLISECS = 134;

            bdlt::Time valueOut(HOUR, MIN, SECS, MILLISECS);

            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            bdlt::Time valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting bdlt::TimeTz"
                               << "\n===================" << bsl::endl;

        {
            const int HOUR   = 12, MIN = 56, SECS = 9, MILLISECS = 134,
                      OFFSET = 45;

            bdlt::TimeTz valueOut(bdlt::Time(HOUR, MIN, SECS, MILLISECS),
                                 OFFSET);

            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            bdlt::TimeTz valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting bdlt::Datetime"
                               << "\n=====================" << bsl::endl;

        {
            const int YEAR = 2005, MONTH = 12, DAY = 15;
            const int HOUR = 12, MIN = 56, SECS = 9, MILLISECS = 134;

            bdlt::Datetime valueOut(YEAR, MONTH, DAY,
                                   HOUR, MIN, SECS, MILLISECS);

            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            bdlt::Datetime valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }

        if (verbose) bsl::cout << "\nTesting bdlt::DatetimeTz"
                               << "\n=======================" << bsl::endl;

        {
            const int YEAR   = 2005, MONTH = 12, DAY = 15;
            const int HOUR   = 12, MIN = 56, SECS = 9, MILLISECS = 134;
            const int OFFSET = 45;

            bdlt::DatetimeTz valueOut(bdlt::Datetime(YEAR, MONTH, DAY,
                                                   HOUR, MIN, SECS, MILLISECS),
                                      OFFSET);

            bdlsb::MemOutStreamBuf osb;
            ASSERT(0 == encoder.encode(&osb, valueOut));

            if (veryVerbose) {
                P(osb.length())
                printBuffer(osb.data(), osb.length());
            }

            bdlt::DatetimeTz valueIn;

            ASSERT(valueOut != valueIn);
            bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
            ASSERT(0 == decoder.decode(&isb, &valueIn));
            printDiagnostic(decoder);

            ASSERT(valueOut == valueIn);
        }
      } break;
      case 2: {
        // --------------------------------------------------------------------
        // REAL TEST
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nREAL TEST"
                               << "\n=========" << bsl::endl;

        {
            const float  XA1 = 99.234f;              float  XA2;
            const float  XB1 = -100.987f;            float  XB2;
            const float  XC1 = -77723.875f;          float  XC2;
            const float  XD1 = 1.4E-45f;             float  XD2;
            const float  XE1 = -1.4E-45f;            float  XE2;
            const float  XF1 = 3.402823466E+38f;     float  XF2;
            const float  XG1 = 1.175494351E-38f;     float  XG2;

            const double XH1 = 19998989.1234;        double  XH2;
            const double XI1 = -7.8752345;           double  XI2;
            const double XJ1 = 1.4E-45;              double  XJ2;
            const double XK1 = -1.4E-45;             double  XK2;
            const double XL1 = 3.402823466E+38;      double  XL2;
            const double XM1 = 1.175494351E-38;      double  XM2;
            const double XN1 = 1e-307;               double  XN2;

            const float  XO1 = 0;                     float XO2;
            const double XP1 = 0;                     double XP2;

            const double XQ1 = -1E-307;                     double XQ2;
            const double XR1 = -1.98347E-296;               double XR2;
            const double XS1 = 78979.23E-304;               double XS2;
            const double XT1 = 79879879249686698E-100;      double XT2;
            const double XU1 = -9999999999999999E+25;       double XU2;

            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XA1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XA2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XA1, XA2, XA1 == XA2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XB1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XB2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XB1, XB2, XB1 == XB2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XC1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XC2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XC1, XC2, XC1 == XC2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XD1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XD2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XD1, XD2, XD1 == XD2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XE1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XE2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XE1, XE2, XE1 == XE2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XF1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XF2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XF1, XF2, XF1 == XF2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XG1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XG2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XG1, XG2, XG1 == XG2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XH1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XH2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XH1, XH2, XH1 == XH2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XI1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XI2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XI1, XI2, XI1 == XI2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XJ1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XJ2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XJ1, XJ2, XJ1 == XJ2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XK1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XK2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XK1, XK2, XK1 == XK2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XL1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XL2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XL1, XL2, XL1 == XL2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XM1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XM2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XM1, XM2, XM1 == XM2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XN1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XN2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XN1, XN2, XN1 == XN2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XO1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XO2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XO1, XO2, XO1 == XO2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XP1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XP2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XP1, XP2, XP1 == XP2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XQ1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XQ2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XQ1, XQ2, XQ1 == XQ2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XR1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XR2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XR1, XR2, XR1 == XR2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XS1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XS2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XS1, XS2, XS1 == XS2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XT1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XT2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XT1, XT2, XT1 == XT2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XU1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XU2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XU1, XU2, XU1 == XU2);
            }
        }
      } break;
      case 1: {
        // --------------------------------------------------------------------
        // FUNDAMENTALS TEST
        //
        // Concerns:
        //
        // Plan:
        //
        // Testing:
        //
        // --------------------------------------------------------------------

        if (verbose) bsl::cout << "\nFUNDAMENTALS TEST"
                               << "\n=================" << bsl::endl;

        {
            const unsigned char   XA1 = UCHAR_MAX;       unsigned char     XA2;
            const signed   char   XB1 = SCHAR_MIN;       signed   char     XB2;
            const          char   XC1 = SCHAR_MAX;                char     XC2;

            const unsigned short  XD1 = USHRT_MAX;    unsigned short    XD2;
            const signed   short  XE1 = SHRT_MIN;     signed   short    XE2;
            const          short  XF1 = SHRT_MAX;              short    XF2;

            const unsigned int    XG1 = UINT_MAX; unsigned int      XG2;
            const signed   int    XH1 = INT_MIN;  signed   int      XH2;
            const          int    XI1 = INT_MAX;           int      XI2;

            const unsigned long   XJ1 = ULONG_MAX; unsigned long     XJ2;
            const signed   long   XK1 = LONG_MIN;  signed   long     XK2;
            const          long   XL1 = LONG_MAX;           long     XL2;

            const bsls::Types::Int64  XM1 = 0xff34567890123456LL;
            bsls::Types::Int64        XM2;
            const bsls::Types::Uint64 XN1 = 0x1234567890123456LL;
            bsls::Types::Uint64       XN2;

            const          bool   XO1 = true;             bool         XO2;

            const bsl::string     XP1("This is a really long line");
            bsl::string           XP2;

            const float        XQ1 = 99.234f;          float        XQ2;
            const float        XR1 = -100.987f;        float        XR2;
            const float        XS1 = -77723.875f;      float        XS2;

            const double       XT1 = 19998989.1234;    double        XT2;
            const double       XU1 = -7.8752345;       double        XU2;

            const double       XV1 = 100.1;              double        XV2;
            const double       XW1 = 200.1;              double        XW2;
            const double       XX1 = 2500.1;             double        XX2;
            const double       XY1 = 1600.1;             double        XY2;
            const double       XZ1 = 100;                double        XZ2;

            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XA1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XA2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XA1, XA2, XA1 == XA2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XB1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XB2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XB1, XB2, XB1 == XB2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XC1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XC2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XC1, XC2, XC1 == XC2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XD1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XD2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XD1, XD2, XD1 == XD2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XE1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XE2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XE1, XE2, XE1 == XE2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XF1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XF2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XF1, XF2, XF1 == XF2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XG1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XG2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XG1, XG2, XG1 == XG2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XH1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XH2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XH1, XH2, XH1 == XH2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XI1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XI2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XI1, XI2, XI1 == XI2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XJ1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XJ2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XJ1, XJ2, XJ1 == XJ2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XK1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XK2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XK1, XK2, XK1 == XK2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XL1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XL2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XL1, XL2, XL1 == XL2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XM1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XM2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XM1, XM2, XM1 == XM2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XN1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XN2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XN1, XN2, XN1 == XN2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XO1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XO2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XO1, XO2, XO1 == XO2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XP1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XP2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XP1, XP2, XP1 == XP2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XQ1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XQ2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XQ1, XQ2, XQ1 == XQ2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XR1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XR2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XR1, XR2, XR1 == XR2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XS1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XS2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XS1, XS2, XS1 == XS2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XT1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XT2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XT1, XT2, XT1 == XT2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XU1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XU2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XU1, XU2, XU1 == XU2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XV1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XV2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XV1, XV2, XV1 == XV2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XW1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XW2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XW1, XW2, XW1 == XW2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XX1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XX2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XX1, XX2, XX1 == XX2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XY1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XY2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XY1, XY2, XY1 == XY2);
            }
            {
                bdlsb::MemOutStreamBuf osb;
                ASSERT(0 == encoder.encode(&osb, XZ1));

                if (veryVerbose) {
                    P(osb.length())
                    printBuffer(osb.data(), osb.length());
                }

                bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());
                ASSERT(0 == decoder.decode(&isb, &XZ2));
                printDiagnostic(decoder);
                LOOP2_ASSERT(XZ1, XZ2, XZ1 == XZ2);
            }
        }
      } break;
      case -1: {
        // --------------------------------------------------------------------
        // PERFORMANCE TEST
        // --------------------------------------------------------------------

        static const bsl::size_t MAX_BUF_SIZE = 100000;

        int reps = 1000;
        int arraySize = 200;
        char requestType = 'b';

        if (argc > 2) {
            ASSERT('-' == argv[2][0]);

            // Request type:
            //  -s == sqrt request
            //  -r == basicRecord request
            //  -b == bigRecord request
            //  -2 == bigRecord2 request
            requestType = argv[2][1];
            if (('b' == requestType || '2' == requestType) && argc > 4) {
                // Get array size for types contain arrays
                arraySize = bsl::atoi(argv[4]);
                veryVeryVerbose = argc > 5;
            }
        }

        if (argc > 3) {
            // Get number of repetitions
            reps = bsl::atoi(argv[3]);
        }

        // Create request object:
        test::TimingRequest request;
        bsl::size_t         minOutputSize;
        switch (requestType) {
          case 's': {
              bsl::cout << "sqrt request" << bsl::endl;

              test::Sqrt sqrt;
              sqrt.value() = 3.1415927;
              request.makeSqrt(sqrt);
              minOutputSize = 2;
          } break;
          case 'r': {
              bsl::cout << "basicRecord request" << bsl::endl;

              test::BasicRecord basicRec;
              basicRec.i1() = 11;
              basicRec.i2() = 22;
              basicRec.dt() = bdlt::DatetimeTz(
                  bdlt::Datetime(bdlt::Date(2007, 9, 3),
                                bdlt::Time(16, 30)), 0);
              basicRec.s() = "The quick brown fox jumped over the lazy dog.";
              request.makeBasic(basicRec);
              minOutputSize = 3 * sizeof(int) + basicRec.s().length();
          } break;
          case 'b': {
              bsl::cout << "bigRecord request with array size of "
                        << arraySize << bsl::endl;

              test::BasicRecord basicRec;
              basicRec.i1() = 11;
              basicRec.i2() = 22;
              basicRec.dt() = bdlt::DatetimeTz(
                  bdlt::Datetime(bdlt::Date(2007, 9, 3),
                                bdlt::Time(16, 30)), 0);
              basicRec.s() = "The quick brown fox jumped over the lazy dog.";
              bsl::size_t minBasicRecSize =
                  minOutputSize = 3 * sizeof(int) + basicRec.s().length();

              test::BigRecord   bigRec;
              bigRec.name() = "This record is so big, it has its own gravity.";

              for (int i = 0; i < arraySize; ++i) {
                  bigRec.array().push_back(basicRec);
              }

              request.makeBig(bigRec);
              minOutputSize =
                  bigRec.name().length() + arraySize * minBasicRecSize;
          } break;
          default:
            bsl::cerr << "Unknown request type: " << '-' << requestType
                      << bsl::endl;
            return 1;                                                 // RETURN
        }

        bsl::cout << "  " << reps << " repetitions..." << bsl::endl;

        bdlsb::MemOutStreamBuf osb;
        osb.reserveCapacity(MAX_BUF_SIZE);

        bsls::Stopwatch stopwatch;
        double elapsed;

        // Measure ber encoding and decoding times:
        stopwatch.reset();
        stopwatch.start();
        for (int i = 0; i < reps; ++i) {
            osb.pubseekpos(0);
            balber::BerEncoder encoder;  // Typical usage: single-use object
            encoder.encode(&osb, request);
        }
        stopwatch.stop();

        ASSERT(minOutputSize <= osb.length());
        ASSERT(osb.length() <= MAX_BUF_SIZE);
        elapsed = stopwatch.elapsedTime();
        ASSERT(elapsed > 0);

        bsl::cout << "    balber::BerEncoder: "
                  << elapsed          << " seconds, "
                  << (reps / elapsed) << " reps/sec, "
                  << osb.length()     << " bytes" << bsl::endl;

        test::TimingRequest *inRequests = 0;
        bdlsb::FixedMemInStreamBuf isb(osb.data(), osb.length());

        isb.pubsetbuf(osb.data(), osb.length());
        inRequests = new test::TimingRequest[reps];
        stopwatch.reset();
        stopwatch.start();
        for (int i = 0; i < reps; ++i) {
            isb.pubseekpos(0);
            balber::BerDecoder decoder;  // Typical usage: single-use object
            decoder.decode(&isb, &inRequests[i]);
        }
        stopwatch.stop();

        if (veryVeryVerbose) { P(request); P(*inRequests); }
        ASSERT(*inRequests == request);
        elapsed = stopwatch.elapsedTime();
        ASSERT(elapsed > 0);
        delete[] inRequests;

        bsl::cout << "    balber::BerDecoder: "
                  << elapsed          << " seconds, "
                  << (reps / elapsed) << " reps/sec" << bsl::endl;

        delete[] inRequests;
      } break;
      default: {
        bsl::cerr << "WARNING: CASE `" << test << "' NOT FOUND." << bsl::endl;
        testStatus = -1;
      }
    }

    if (testStatus > 0) {
        bsl::cerr << "Error, non-zero test status = " << testStatus << "."
                  << bsl::endl;
    }

    return testStatus;
}

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
