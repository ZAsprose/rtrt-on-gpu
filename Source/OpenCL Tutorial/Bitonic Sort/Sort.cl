/*
 * For a description of the algorithm and the terms used, please see the
 * documentation for this sample.
 *
 * One invocation of this kernel, i.e one work thread writes two output values.
 * Since every pass of this algorithm does width/2 comparisons, each compare
 * operation is done by one work thread.
 * 
 * Depending of the direction of sort for the work thread, the output values
 * are written either as greater value to left element or lesser value to the
 * left element. Right element and left element are the two elements we are 
 * comparing and "left" is the element with a smaller index into the array.
 *
 * if direction is CL_TRUE, i.e evaluates to non zero, it means "increasing".
 *
 * For an explanation of the terms "blockWidth", "sameDirectionBlockWidth",
 * stage, pass, pairDistance please go through the document shipped with this
 * sample.
 *
 * Since an explanation of the terms and the code here would be quite lengthy,
 * confusing and will greatly reduce the readability of this kernel, the code 
 * has been explained in detail in the document mentioned above.
 */

__kernel 
void Sort ( __global uint * theArray,
            const uint stage, 
            const uint passOfStage,
            const uint width,
            const uint direction )
{
    uint sortIncreasing = direction;
    uint threadId = get_global_id(0);
    
    uint pairDistance = 1 << (stage - passOfStage);
    uint blockWidth   = 2 * pairDistance;

    uint leftId = (threadId % pairDistance) 
                   + (threadId / pairDistance) * blockWidth;

    uint rightId = leftId + pairDistance;
    
    uint leftElement = theArray[leftId];
    uint rightElement = theArray[rightId];
    
    uint sameDirectionBlockWidth = 1 << stage;
    
    sortIncreasing = ( threadId / sameDirectionBlockWidth ) % 2 == 1 ?
        1 - sortIncreasing : sortIncreasing;


    uint greater = leftElement > rightElement ? leftElement : rightElement;

    uint lesser = leftElement > rightElement ? rightElement : leftElement;


    theArray [leftId] = sortIncreasing ? lesser : greater;
    
    theArray [rightId] = sortIncreasing ? greater : lesser;
}