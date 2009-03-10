#include "cppunit/TestFixture.h"
#include "cppunit/TestResult.h"
#include "cppunit/ui/text/TestRunner.h"
#include "cppunit/extensions/HelperMacros.h"

#include "Vector2D.h"

using namespace Math;

/* Vector2D unit test */

class Vector2DTest : public CppUnit::TestFixture {
private:
  Vector2D *v1, *v2, *v3;
public:
  void setUp()
  {
    v1 = new Vector2D( 1.0f, -2.5f );
	float params[] = { -5.5f, 6.5f };
	v2 = new Vector2D( params );
	v3 = new Vector2D( 0.0f, 0.0f );
  }

  void tearDown() 
  {
    delete v1;
    delete v2;
	delete v3;
  }

  void testEquality()
  {
    CPPUNIT_ASSERT( *v1 == *v1 );
	CPPUNIT_ASSERT( *v2 == * new Vector2D( -5.5f, 6.5f ) );
	CPPUNIT_ASSERT( *v3 == *v3 );
    CPPUNIT_ASSERT( !(*v1 == *v3) );

	CPPUNIT_ASSERT( *v1 != *v3 );
	CPPUNIT_ASSERT( !(*v1 != *v1) );

	CPPUNIT_ASSERT( 1 == * new Vector2D( 1.0f, 1.0f ) );
	CPPUNIT_ASSERT( * new Vector2D( 1.0f, 1.0f ) == 1 );

	CPPUNIT_ASSERT( 1 != * new Vector2D( 1.0f, 2.0f ) );
	CPPUNIT_ASSERT( * new Vector2D( 1.0f, 2.0f ) != 1 );
  }

  void testAuxiliaryOperators()
  {
	  Vector2D localV( 1.0f, 2.0f );
	  CPPUNIT_ASSERT( localV[0] == 1.0f );
	  CPPUNIT_ASSERT( localV[1] == 2.0f );
		
	  localV[0] = 3.0f;
	  localV[1] = 4.0f;

	  CPPUNIT_ASSERT( localV == * new Vector2D(3.0f, 4.0f) );

	  CPPUNIT_ASSERT( *localV == 3.0f );	
  }

  void testAddition()
  {
    CPPUNIT_ASSERT( *v2 + *v2 == * new Vector2D( -11.0f, 13.0f ) );
	CPPUNIT_ASSERT( *v2 - *v2 == Vector2D::Zero );
	
	CPPUNIT_ASSERT( *v2 + 1.0f == * new Vector2D( -4.5f, 7.5f ) );
	CPPUNIT_ASSERT( *v2 - 1.0f == * new Vector2D( -6.5f, 5.5f ) );
	
	CPPUNIT_ASSERT( 1.0f + *v2 == * new Vector2D( -4.5f, 7.5f ) );
	CPPUNIT_ASSERT( 1.0f - *v2 == * new Vector2D( 6.5f, -5.5f ) );

	Vector2D localV( 1.0f, 2.0f );
	CPPUNIT_ASSERT( (localV += Vector2D( 1.0f, 2.0f )) == * new Vector2D( 2.0f, 4.0f ) );
	CPPUNIT_ASSERT( (localV -= Vector2D( 1.0f, 2.0f )) == * new Vector2D( 1.0f, 2.0f ) );

	CPPUNIT_ASSERT( (localV += 1.0f) == * new Vector2D( 2.0f, 3.0f ) );
	CPPUNIT_ASSERT( (localV -= 1.0f) == * new Vector2D( 1.0f, 2.0f ) );
  }

  void testMultiplication()
  {
    CPPUNIT_ASSERT( *v2 * *v2 == * new Vector2D( 5.5f * 5.5f, 6.5f * 6.5f ) );
	CPPUNIT_ASSERT( *v2 / *v2 == Vector2D::Unit );
	
	CPPUNIT_ASSERT( *v2 * 2.0f == * new Vector2D( -5.5f * 2.0f, 6.5f * 2.0f ) );
	CPPUNIT_ASSERT( *v2 / 2.0f == * new Vector2D( -5.5f / 2.0f, 6.5f / 2.0f ) );

	CPPUNIT_ASSERT( 2.0f * *v2 == * new Vector2D( -5.5f * 2.0f, 6.5f * 2.0f ) );
	CPPUNIT_ASSERT( 2.0f / *v2  == * new Vector2D( 2.0f / -5.5f, 2.0f / 6.5f ) );
	
	Vector2D localV( 1.0f, 2.0f );
	CPPUNIT_ASSERT( (localV *= Vector2D( 1.0f, 2.0f )) == * new Vector2D( 1.0f, 4.0f ) );
	CPPUNIT_ASSERT( (localV /= Vector2D( 1.0f, 2.0f )) == * new Vector2D( 1.0f, 2.0f ) );

	CPPUNIT_ASSERT( (localV *= 2.0f) == * new Vector2D( 2.0f, 4.0f ) );
	CPPUNIT_ASSERT( (localV /= 2.0f) == * new Vector2D( 1.0f, 2.0f ) );
  }

  void testComparison()
  {
	  Vector2D localV( 1.0f, 2.0f );
	  CPPUNIT_ASSERT( localV > * new Vector2D( 0.9f, 1.9f ) );
	  CPPUNIT_ASSERT( !(localV > * new Vector2D( 1.9f, 1.9f )) );

	  CPPUNIT_ASSERT( localV > 0.9f );
	  CPPUNIT_ASSERT( !(localV > 2) );

	  CPPUNIT_ASSERT( 3 > localV );
	  CPPUNIT_ASSERT( !(2 > localV) );

	  CPPUNIT_ASSERT( localV < * new Vector2D( 1.1f, 2.1f ) );
	  CPPUNIT_ASSERT( !(localV < * new Vector2D( 0.9f, 2.1f )) );

	  CPPUNIT_ASSERT( localV < 3 );
	  CPPUNIT_ASSERT( !(localV < 2) );

	  CPPUNIT_ASSERT( 0.9f < localV );
	  CPPUNIT_ASSERT( !(2 < localV ) );


	  CPPUNIT_ASSERT( localV >= * new Vector2D( 1.0f, 1.9f ) );
	  CPPUNIT_ASSERT( !(localV >= * new Vector2D( 1.9f, 1.9f )) );

	  CPPUNIT_ASSERT( localV >= 1.0f );
	  CPPUNIT_ASSERT( !(localV >= 3.0f) );

	  CPPUNIT_ASSERT( 2 >= localV );
	  CPPUNIT_ASSERT( !(1 >= localV) );

	  CPPUNIT_ASSERT( localV <= * new Vector2D( 1.0f, 2.1f ) );
	  CPPUNIT_ASSERT( !(localV <= * new Vector2D( 0.9f, 2.0f )) );

	  CPPUNIT_ASSERT( localV <= 2 );
	  CPPUNIT_ASSERT( !(localV <= 1) );

	  CPPUNIT_ASSERT( 1.0f <= localV );
	  CPPUNIT_ASSERT( !(3.0 <= localV ) );
  }

  void testCommonFunctions()
  {
	  Vector2D localV( -1.0f, 1.0f );
	  
	  CPPUNIT_ASSERT( Abs( localV ) == * new Vector2D( 1.0f, 1.0f ) );	

	  CPPUNIT_ASSERT( Sign( localV ) == * new Vector2D( -1.0f, 1.0f ) );	
	  CPPUNIT_ASSERT( Sign( * new Vector2D( 0.0f, 0.0f ) ) == * new Vector2D( 0.0f, 0.0f ) );	
	  CPPUNIT_ASSERT( Sign( * new Vector2D( 0.0f, 100.0f ) ) == * new Vector2D( 0.0f, 1.0f ) );	
	  
	  // TODO: implement
  }

  void testGeometricFunctions()
  {
	  Vector2D localV( -3.0f, 4.0f );
	  
	  CPPUNIT_ASSERT( 5 == Length( localV ) );

	  CPPUNIT_ASSERT( 25 == Square( localV ) );

	  CPPUNIT_ASSERT( * new Vector2D( -4.0f, 3.0f ) == localV - Vector2D::Unit );
	  CPPUNIT_ASSERT( 5 == Distance( localV, Vector2D::Unit ) );

	  CPPUNIT_ASSERT( 25 == Dot( localV, localV ) );

	  CPPUNIT_ASSERT( * new Vector2D( -3.0f / 5, 4.0f / 5 ) == Normalize( localV ) ); 
	  
	  //TODO: implement test for reflect and refract
  }

  /* Vector2D test suite */

  CPPUNIT_TEST_SUITE( Vector2DTest );

  CPPUNIT_TEST( testEquality );
  CPPUNIT_TEST( testAddition );
  CPPUNIT_TEST( testMultiplication );
  CPPUNIT_TEST( testComparison );
  CPPUNIT_TEST( testAuxiliaryOperators );
  CPPUNIT_TEST( testCommonFunctions );
  CPPUNIT_TEST( testGeometricFunctions );

  CPPUNIT_TEST_SUITE_END();

  /* end of Vector2D test suite */
};

void main( void )
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( Vector2DTest::suite() );
  runner.run();
}