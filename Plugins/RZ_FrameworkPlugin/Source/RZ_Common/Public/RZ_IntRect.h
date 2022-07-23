#pragma once

#include "CoreTypes.h"
#include "Math/UnrealMathUtility.h"
#include "Containers/UnrealString.h"
#include "Math/IntPoint.h"
#include "Math/Vector2D.h"
#include "RZ_IntRect.generated.h"

/**
 * Structure for integer rectangles in 2-d space.
 *
 * @todo Docs: The operators need better documentation, i.e. what does it mean to divide a rectangle?
 */
USTRUCT(BlueprintType)
struct FRZ_IntRect
{
	GENERATED_USTRUCT_BODY()
	
	/** Holds the first pixel line/row (like in Win32 RECT). */
	FIntPoint Min;

	/** Holds the last pixel line/row (like in Win32 RECT). */
	FIntPoint Max;

public:
	
	/** Constructor */
	FRZ_IntRect();

	/**
	 * Constructor
	 *
	 * @param X0 Minimum X coordinate.
	 * @param Y0 Minimum Y coordinate.
	 * @param X1 Maximum X coordinate.
	 * @param Y1 Maximum Y coordinate.
	 */
	FRZ_IntRect(int32 X0, int32 Y0, int32 X1, int32 Y1);

	/**
	 * Constructor
	 *
	 * @param StartPoint
	 * @param EndPoint
	 */
	FRZ_IntRect(FIntPoint StartPoint, FIntPoint EndPoint);

public:

	/**
	 * Gets a specific point in this rectangle.
	 *
	 * @param PointIndex Index of Point in rectangle.
	 * @return Const reference to point in rectangle.
	 */
	const FIntPoint& operator()( int32 PointIndex ) const;

	/**
	 * Gets a specific point in this rectangle.
	 *
	 * @param PointIndex Index of Point in rectangle.
	 * @return Reference to point in rectangle.
	 */
	FIntPoint& operator()( int32 PointIndex );

	/**
	 * Compares Rectangles for equality.
	 *
	 * @param Other The Other Rectangle for comparison.
	 * @return true if the rectangles are equal, false otherwise..
	 */
	bool operator==( const FRZ_IntRect& Other ) const;

	/**
	 * Compares Rectangles for inequality.
	 *
	 * @param Other The Other Rectangle for comparison.
	 * @return true if the rectangles are not equal, false otherwise..
	 */
	bool operator!=( const FRZ_IntRect& Other ) const;

	/**
	 * Applies scaling to this rectangle.
	 *
	 * @param Scale What to multiply the rectangle by.
	 * @return Reference to this rectangle after scaling.
	 */
	FRZ_IntRect& operator*=( int32 Scale );

	/**
	 * Adds a point to this rectangle.
	 *
	 * @param Point The point to add onto both points in the rectangle.
	 * @return Reference to this rectangle after addition.
	 */
	FRZ_IntRect& operator+=( const FIntPoint& Point );

	/**
	 * Subtracts a point from this rectangle.
	 *
	 * @param Point The point to subtract from both points in the rectangle.
	 * @return Reference to this rectangle after subtraction.
	 */
	FRZ_IntRect& operator-=( const FIntPoint& Point );

	/**
	 * Gets the result of scaling on this rectangle.
	 *
	 * @param Scale What to multiply this rectangle by.
	 * @return New scaled rectangle.
	 */
	FRZ_IntRect operator*( int32 Scale ) const;

	/**
	 * Gets the result of division on this rectangle.
	 *
	 * @param Div What to divide this rectangle by.
	 * @return New divided rectangle.
	 */
	FRZ_IntRect operator/( int32 Div ) const;

	/**
	 * Gets the result of adding a point to this rectangle.
	 *
	 * @param Point The point to add to both points in the rectangle.
	 * @return New rectangle with point added to it.
	 */
	FRZ_IntRect operator+( const FIntPoint& Point ) const;

	/**
	 * Gets the result of dividing a point with this rectangle.
	 *
	 * @param Point The point to divide with.
	 * @return New rectangle with point divided.
	 */
	FRZ_IntRect operator/( const FIntPoint& Point ) const;

	/**
	 * Gets the result of subtracting a point from this rectangle.
	 *
	 * @param Point The point to subtract from both points in the rectangle.
	 * @return New rectangle with point subtracted from it.
	 */
	FRZ_IntRect operator-( const FIntPoint& Point ) const;

	/**
	 * Gets the result of adding two rectangles together.
	 *
	 * @param Other The other rectangle to add to this.
	 * @return New rectangle after both are added together.
	 */
	FRZ_IntRect operator+( const FRZ_IntRect& Other ) const;

	/**
	 * Gets the result of subtracting a rectangle from this one.
	 *
	 * @param Other The other rectangle to subtract from this.
	 * @return New rectangle after one is subtracted from this.
	 */
	FRZ_IntRect operator-( const FRZ_IntRect& Other ) const;

public:

	/**
	 * Calculates the area of this rectangle.
	 *
	 * @return The area of this rectangle.
	 */
	int32 Area() const;

	/**
	 * Creates a rectangle from the bottom part of this rectangle.
	 *
	 * @param InHeight Height of the new rectangle (<= rectangles original height).
	 * @return The new rectangle.
	 */
	FRZ_IntRect Bottom( int32 InHeight ) const;

	/**
	 * Clip a rectangle using the bounds of another rectangle.
	 *
	 * @param Other The other rectangle to clip against.
	 */
	void Clip( const FRZ_IntRect& Other );

	/** Combines the two rectanges. */
	void Union( const FRZ_IntRect& Other);

	/**
	 * Test whether this rectangle contains a point.
	 *
	 * @param Point The point to test against.
	 * @return true if the rectangle contains the specified point,, false otherwise..
	 */
	bool Contains( FIntPoint Point ) const;

	/**
	 * Gets the Center and Extents of this rectangle.
	 *
	 * @param OutCenter Will contain the center point.
	 * @param OutExtent Will contain the extent.
	 */
	void GetCenterAndExtents( FIntPoint& OutCenter, FIntPoint& OutExtent ) const;

	/**
	 * Gets the Height of the rectangle.
	 *
	 * @return The Height of the rectangle.
	 */
	int32 Height() const;

	/**
	 * Inflates or deflates the rectangle.
	 *
	 * @param Amount The amount to inflate or deflate the rectangle on each side.
	 */
	void InflateRect( int32 Amount );

	/**
	 * Adds to this rectangle to include a given point.
	 *
	 * @param Point The point to increase the rectangle to.
	 */
	void Include( FIntPoint Point );

	/**
	 * Gets a new rectangle from the inner of this one.
	 *
	 * @param Shrink How much to remove from each point of this rectangle.
	 * @return New inner Rectangle.
	 */
	FRZ_IntRect Inner( FIntPoint Shrink ) const;

	/**
	 * Creates a rectangle from the right hand side of this rectangle.
	 *
	 * @param InWidth Width of the new rectangle (<= rectangles original width).
	 * @return The new rectangle.
	 */
	FRZ_IntRect Right( int32 InWidth ) const;

	/**
	 * Scales a rectangle using a floating point number.
	 *
	 * @param Fraction What to scale the rectangle by
	 * @return New scaled rectangle.
	 */
	FRZ_IntRect Scale( float Fraction ) const;

	/**
	 * Gets the distance from one corner of the rectangle to the other.
	 *
	 * @return The distance from one corner of the rectangle to the other.
	 */
	FIntPoint Size() const;

	/**
	 * Get a textual representation of this rectangle.
	 *
	 * @return A string describing the rectangle.
	 */
	FString ToString() const;

	/**
	 * Gets the width of the rectangle.
	 *
	 * @return The width of the rectangle.
	 */
	int32 Width() const;

	/**
	 * Returns true if the rectangle is 0 x 0.
	 *
	 * @return true if the rectangle is 0 x 0.
	 */
	bool IsEmpty() const;

public:

	/**
	 * Divides a rectangle and rounds up to the nearest integer.
	 *
	 * @param lhs The Rectangle to divide.
	 * @param Div What to divide by.
	 * @return New divided rectangle.
	 */
	static FRZ_IntRect DivideAndRoundUp( FRZ_IntRect lhs, int32 Div );
	static FRZ_IntRect DivideAndRoundUp( FRZ_IntRect lhs, FIntPoint Div );

	/**
	 * Gets number of points in the Rectangle.
	 *
	 * @return Number of points in the Rectangle.
	 */
	static int32 Num();

public:

	/**
	 * Serializes the Rectangle.
	 *
	 * @param Ar The archive to serialize into.
	 * @param Rect The rectangle to serialize.
	 * @return Reference to the Archive after serialization.
	 */
	friend FArchive& operator<<( FArchive& Ar, FRZ_IntRect& Rect )
	{
		return Ar << Rect.Min.X << Rect.Min.Y << Rect.Max.X << Rect.Max.Y;
	}
};


FORCEINLINE FRZ_IntRect FRZ_IntRect::Scale( float Fraction ) const
{
	FVector2D Min2D = FVector2D((float)Min.X, (float)Min.Y) * Fraction;
	FVector2D Max2D = FVector2D((float)Max.X, (float)Max.Y) * Fraction;

	return FRZ_IntRect(FMath::FloorToInt(Min2D.X), FMath::FloorToInt(Min2D.Y), FMath::CeilToInt(Max2D.X), FMath::CeilToInt(Max2D.Y));
}


/* FRZ_IntRect inline functions
 *****************************************************************************/

FORCEINLINE FRZ_IntRect::FRZ_IntRect()
	: Min(ForceInit)
	, Max(ForceInit)
{ }


FORCEINLINE FRZ_IntRect::FRZ_IntRect( int32 X0, int32 Y0, int32 X1, int32 Y1 )
	: Min(X0, Y0)
	, Max(X1, Y1)
{ }


FORCEINLINE FRZ_IntRect::FRZ_IntRect( FIntPoint StartPoint, FIntPoint EndPoint )
{
	StartPoint.X < EndPoint.X
		? Min.X = StartPoint.X
		: Min.X = EndPoint.X;

	StartPoint.X > EndPoint.X
		? Max.X = StartPoint.X
		: Max.X = EndPoint.X;

	StartPoint.Y < EndPoint.Y
		? Min.Y = StartPoint.Y
		: Min.Y = EndPoint.Y;

	StartPoint.Y > EndPoint.Y
		? Max.Y = StartPoint.Y
		: Max.Y = EndPoint.Y;
}


FORCEINLINE const FIntPoint& FRZ_IntRect::operator()( int32 PointIndex ) const
{
	return (&Min)[PointIndex];
}


FORCEINLINE FIntPoint& FRZ_IntRect::operator()( int32 PointIndex )
{
	return (&Min)[PointIndex];
}


FORCEINLINE bool FRZ_IntRect::operator==( const FRZ_IntRect& Other ) const
{
	return Min==Other.Min && Max==Other.Max;
}


FORCEINLINE bool FRZ_IntRect::operator!=( const FRZ_IntRect& Other ) const
{
	return Min!=Other.Min || Max!=Other.Max;
}


FORCEINLINE FRZ_IntRect& FRZ_IntRect::operator*=( int32 Scale )
{
	Min *= Scale;
	Max *= Scale;

	return *this;
}


FORCEINLINE FRZ_IntRect& FRZ_IntRect::operator+=( const FIntPoint& Point )
{
	Min += Point;
	Max += Point;

	return *this;
}


FORCEINLINE FRZ_IntRect& FRZ_IntRect::operator-=( const FIntPoint& Point )
{
	Min -= Point;
	Max -= Point;

	return *this;
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator*( int32 Scale ) const
{
	return FRZ_IntRect(Min * Scale, Max * Scale);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator/( int32 Div ) const
{
	return FRZ_IntRect(Min / Div, Max / Div);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator+( const FIntPoint& Point ) const
{
	return FRZ_IntRect(Min + Point, Max + Point);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator/( const FIntPoint& Point ) const
{
	return FRZ_IntRect(Min / Point, Max / Point);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator-( const FIntPoint& Point ) const
{
	return FRZ_IntRect(Min - Point, Max - Point);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator+( const FRZ_IntRect& Other ) const
{
	return FRZ_IntRect(Min + Other.Min, Max + Other.Max);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::operator-( const FRZ_IntRect& Other ) const
{
	return FRZ_IntRect(Min - Other.Min, Max - Other.Max);
}


FORCEINLINE int32 FRZ_IntRect::Area() const
{
	return (Max.X - Min.X) * (Max.Y - Min.Y);
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::Bottom( int32 InHeight ) const
{
	return FRZ_IntRect( Min.X, FMath::Max(Min.Y,Max.Y-InHeight), Max.X, Max.Y );
}


FORCEINLINE void FRZ_IntRect::Clip( const FRZ_IntRect& R )
{
	Min.X = FMath::Max<int32>(Min.X, R.Min.X);
	Min.Y = FMath::Max<int32>(Min.Y, R.Min.Y);
	Max.X = FMath::Min<int32>(Max.X, R.Max.X);
	Max.Y = FMath::Min<int32>(Max.Y, R.Max.Y);

	// return zero area if not overlapping
	Max.X = FMath::Max<int32>(Min.X, Max.X);
	Max.Y = FMath::Max<int32>(Min.Y, Max.Y);
}

FORCEINLINE void FRZ_IntRect::Union( const FRZ_IntRect& R )
{
	Min.X = FMath::Min<int32>(Min.X, R.Min.X);
	Min.Y = FMath::Min<int32>(Min.Y, R.Min.Y);
	Max.X = FMath::Max<int32>(Max.X, R.Max.X);
	Max.Y = FMath::Max<int32>(Max.Y, R.Max.Y);
}

FORCEINLINE bool FRZ_IntRect::Contains( FIntPoint P ) const
{
	return P.X >= Min.X && P.X < Max.X && P.Y >= Min.Y && P.Y < Max.Y;
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::DivideAndRoundUp( FRZ_IntRect lhs, int32 Div )
{
	return DivideAndRoundUp(lhs, FIntPoint(Div, Div));
}

FORCEINLINE FRZ_IntRect FRZ_IntRect::DivideAndRoundUp( FRZ_IntRect lhs, FIntPoint Div )
{
	return FRZ_IntRect(lhs.Min / Div, FIntPoint::DivideAndRoundUp(lhs.Max, Div));
}

FORCEINLINE void FRZ_IntRect::GetCenterAndExtents( FIntPoint& OutCenter, FIntPoint& OutExtent ) const
{
	OutExtent.X = (Max.X - Min.X) / 2;
	OutExtent.Y = (Max.Y - Min.Y) / 2;

	OutCenter.X = Min.X + OutExtent.X;
	OutCenter.Y = Min.Y + OutExtent.Y;
}


FORCEINLINE int32 FRZ_IntRect::Height() const
{
	return (Max.Y - Min.Y);
}


FORCEINLINE void FRZ_IntRect::InflateRect( int32 Amount )
{
	Min.X -= Amount;
	Min.Y -= Amount;
	Max.X += Amount;
	Max.Y += Amount;
}


FORCEINLINE void FRZ_IntRect::Include( FIntPoint Point )
{
	Min.X = FMath::Min(Min.X, Point.X);
	Min.Y = FMath::Min(Min.Y, Point.Y);
	Max.X = FMath::Max(Max.X, Point.X);
	Max.Y = FMath::Max(Max.Y, Point.Y);
}

FORCEINLINE FRZ_IntRect FRZ_IntRect::Inner( FIntPoint Shrink ) const
{
	return FRZ_IntRect(Min + Shrink, Max - Shrink);
}


FORCEINLINE int32 FRZ_IntRect::Num()
{
	return 2;
}


FORCEINLINE FRZ_IntRect FRZ_IntRect::Right( int32 InWidth ) const
{
	return FRZ_IntRect( FMath::Max(Min.X, Max.X - InWidth), Min.Y, Max.X, Max.Y );
}


FORCEINLINE FIntPoint FRZ_IntRect::Size() const
{
	return FIntPoint( Max.X-Min.X, Max.Y-Min.Y );
}


FORCEINLINE FString FRZ_IntRect::ToString() const
{
	return FString::Printf(TEXT("Min=(%s) Max=(%s)"), *Min.ToString(), *Max.ToString());
}


FORCEINLINE int32 FRZ_IntRect::Width() const
{
	return Max.X-Min.X;
}

FORCEINLINE bool FRZ_IntRect::IsEmpty() const
{
	return Width() == 0 && Height() == 0;
}

class RZ_IntRect
{
public:

	static FORCEINLINE TArray<FRZ_IntRect> GetAllSquaresIncludingPosition(const FIntPoint& Position, int32 SquareSize)
	{
		TArray<FRZ_IntRect> ResultArray;
		FIntPoint MinPosition;
		MinPosition.X = Position.X - SquareSize + 1;
		MinPosition.Y = Position.Y - SquareSize + 1;

		FIntPoint Index;
		for (Index.X = MinPosition.X; Index.X <= Position.X; Index.X++)
		{
			for (Index.Y = MinPosition.Y; Index.Y <= Position.Y; Index.Y++)
			{
				FRZ_IntRect NewIntRect;
				NewIntRect.Min.X = Index.X;
				NewIntRect.Min.Y = Index.Y;
				NewIntRect.Max.X = Index.X + SquareSize - 1;
				NewIntRect.Max.Y = Index.Y + SquareSize - 1;
				
				ResultArray.Add(NewIntRect);
			}
		}

		return ResultArray;
	}

	static FORCEINLINE TArray<FRZ_IntRect> GetAllSquaresIncludingPositionFromTopLeft(const FIntPoint& Position, int32 SquareSize)
	{
		TArray<FRZ_IntRect> ResultArray;
		FIntPoint MinPosition;
		MinPosition.X = Position.X - SquareSize + 1;
		MinPosition.Y = Position.Y - SquareSize + 1;

		FIntPoint Index;
		for (Index.X = Position.X; Index.X >= MinPosition.X; Index.X--)
		{
			for (Index.Y = MinPosition.Y; Index.Y <= Position.Y; Index.Y++)
			{
				FRZ_IntRect NewIntRect;
				NewIntRect.Min.X = Index.X;
				NewIntRect.Min.Y = Index.Y;
				NewIntRect.Max.X = Index.X + SquareSize - 1;
				NewIntRect.Max.Y = Index.Y + SquareSize - 1;
				
				ResultArray.Add(NewIntRect);
			}
		}

		return ResultArray;
	}

	static FORCEINLINE void GetRectContainingRects(FRZ_IntRect& RectResult, const TArray<FRZ_IntRect>& ContainedRects)
	{
		if (ContainedRects.Num() == 0)
			return;

		RectResult = ContainedRects[0];
		
		for (const auto& Rect : ContainedRects)
		{
			if (Rect.Min.X < RectResult.Min.X)
				RectResult.Min.X = Rect.Min.X;
			if (Rect.Min.Y < RectResult.Min.Y)
				RectResult.Min.Y = Rect.Min.Y;
			if (Rect.Max.X > RectResult.Max.X)
				RectResult.Max.X = Rect.Max.X;
			if (Rect.Max.Y > RectResult.Max.Y)
				RectResult.Max.Y = Rect.Max.Y;
		}
	}
};