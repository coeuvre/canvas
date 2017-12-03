#ifndef CANVAS_CANVAS_H
#define CANVAS_CANVAS_H

#define CANVASAPI extern

// HTML Canvas Spec https://html.spec.whatwg.org/multipage/canvas.html

// ----------------------------------------------------------------------------
// Context

typedef struct CanvasRenderingContext2D CanvasRenderingContext2D;

CANVASAPI CanvasRenderingContext2D *canvasCreateContext(void);

CANVASAPI void canvasDestroyContext(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Canvas state

// Pushes the current state onto the stack.
CANVASAPI void canvasSave(CanvasRenderingContext2D *ctx);

// Pops the top state on the stack, restoring the context to that state.
CANVASAPI void canvasRestore(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Transformations

typedef struct CanvasMatrix2D CanvasMatrix2D;
struct CanvasMatrix2D {
  float a, b, c, d, e, f;
};

// Changes the current transformation matrix to apply a scaling transformation
// with the given characteristics.
CANVASAPI void canvasScale(CanvasRenderingContext2D *ctx, float x, float y);

// Changes the current transformation matrix to apply a rotation transformation
// with the given characteristics. The angle is in radians.
CANVASAPI void canvasRotate(CanvasRenderingContext2D *ctx, float angle);

// Changes the current transformation matrix to apply a translation
// transformation with the given characteristics.
CANVASAPI void canvasTranslate(CanvasRenderingContext2D *ctx, float x, float y);

// Changes the current transformation matrix to apply the matrix given by the
// arguments as described below.
CANVASAPI void canvasTransform(CanvasRenderingContext2D *ctx, float a, float b,
                               float c, float d, float e, float f);

// Returns a copy of the current transformation matrix, as a newly created
// DOMMatrix object.
CANVASAPI CanvasMatrix2D canvasGetTransform(CanvasRenderingContext2D *ctx);

// Changes the current transformation matrix to the matrix given by the
// arguments as described below.
CANVASAPI void canvasSetTransform(CanvasRenderingContext2D *ctx, float a,
                                  float b, float c, float d, float e, float f);

// Changes the current transformation matrix to the matrix represented by the
// passed DOMMatrix2DInit object.
CANVASAPI void canvasSetTransformM2(CanvasRenderingContext2D *ctx,
                                    CanvasMatrix2D transform);

// Changes the current transformation matrix to the identity transform.
CANVASAPI void canvasResetTransform(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Compositing

// Change the alpha value. Values outside of the range 0.0 .. 1.0 are ignored.
CANVASAPI void canvasSetGlobalAlpha(CanvasRenderingContext2D *ctx,
                                    float globalAlpha);

// Returns the current alpha value applied to rendering operations.
CANVASAPI float canvasGetGlobalAlpha(CanvasRenderingContext2D *ctx);

typedef enum CanvasCompositeOperation CanvasCompositeOperation;
enum CanvasCompositeOperation {
  // A atop B.
  //
  // Display the source image wherever both images are opaque. Display the
  // destination image wherever the destination image is opaque but the source
  // image is transparent. Display transparency elsewhere.
  CANVAS_COMPOSITE_OPERATION_SOURCE_ATOP,

  // A in B.
  //
  // Display the source image wherever both the source image and destination
  // image are opaque. Display transparency elsewhere.
  CANVAS_COMPOSITE_OPERATION_SOURCE_IN,

  // A out B.
  //
  // Display the source image wherever the source image is opaque and the
  // destination image is transparent. Display transparency elsewhere.
  CANVAS_COMPOSITE_OPERATION_SOURCE_OUT,

  // A over B (default).
  //
  // Display the source image wherever the source image is opaque. Display the
  // destination image elsewhere.
  CANVAS_COMPOSITE_OPERATION_SOURCE_OVER,

  // B atop A.
  //
  // Same as CANVAS_COMPOSITE_OPERATION_SOURCE_ATOP but using the destination
  // image instead of the source image and vice versa.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_ATOP,

  // B in A.
  //
  // Same as CANVAS_COMPOSITE_OPERATION_SOURCE_IN but using the destination
  // image instead of the source image and vice versa.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_IN,

  // B out A.
  //
  // Same as CANVAS_COMPOSITE_OPERATION_SOURCE_OUT but using the destination
  // image instead of the source image and vice versa.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_OUT,

  // B over A.
  //
  // Same as CANVAS_COMPOSITE_OPERATION_SOURCE_OVER but using the destination
  // image instead of the source image and vice versa.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_OVER,

  // A plus B.
  //
  // Display the sum of the source image and destination image, with color
  // values approaching 255 (100%) as a limit.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_LIGHTER,

  // A (B is ignored).
  //
  // Display the source image instead of the destination image.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_COPY,

  // A xor B.
  //
  // Exclusive OR of the source image and destination image.
  CANVAS_COMPOSITE_OPERATION_DESTINATION_XOR,
};

// Returns the current composition operation
CANVASAPI void canvasSetGlobalCompositeOperation(
    CanvasRenderingContext2D *ctx,
    CanvasCompositeOperation globalCompositeOperation);

// Change the composition operation. Unknown values are ignored.
CANVASAPI CanvasCompositeOperation
canvasGetGlobalCompositeOperation(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Image smoothing

// Change whether images are smoothed (true) or not (false).
CANVASAPI void canvasSetImageSmoothingEnabled(CanvasRenderingContext2D *ctx,
                                              int imageSmoothingEnabled);

// Returns whether pattern fills and the drawImage() method will attempt to
// smooth images if their pixels don't line up exactly with the display, when
// scaling images up.
CANVASAPI int canvasGetImageSmoothingEnabled(CanvasRenderingContext2D *ctx);

typedef enum CanvasImageSmoothingQuality CanvasImageSmoothingQuality;
enum CanvasImageSmoothingQuality {
  CANVAS_IMAGE_SMOOTHING_QUANLITY_LOW,
  CANVAS_IMAGE_SMOOTHING_QUANLITY_MEDIUM,
  CANVAS_IMAGE_SMOOTHING_QUANLITY_HIGH,
};

// Change the preferred quality of image smoothing. The possible values are
// "low", "medium" and "high". Unknown values are ignored.
CANVASAPI void canvasSetImageSmoothingQuality(
    CanvasRenderingContext2D *ctx,
    CanvasImageSmoothingQuality imageSmoothingQuality);

// Returns the current image-smoothing-quality preference.
CANVASAPI CanvasImageSmoothingQuality
canvasGetImageSmoothingQuality(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Fill and stroke styles

typedef struct CanvasColor CanvasColor;
typedef struct CanvasPattern CanvasPattern;
typedef struct CanvasPattern CanvasGradient;

typedef struct CanvasStyle CanvasStyle;
struct CanvasStyle {
  int type;
  union {
    CanvasColor *color;
    CanvasPattern *pattern;
    CanvasGradient *gradient;
  };
};

// Change the fill style.
CANVASAPI void canvasSetFillStyle(CanvasRenderingContext2D *ctx,
                                  CanvasStyle style);

// Returns the current style used for filling shapes.
CANVASAPI CanvasStyle canvasGetFillStyle(CanvasRenderingContext2D *ctx);

// Change the stroke style.
CANVASAPI void canvasSetStrokeStyle(CanvasRenderingContext2D *ctx,
                                    CanvasStyle style);

// Returns the current style used for stroking shapes.
CANVASAPI CanvasStyle canvasGetStrokeStyle(CanvasRenderingContext2D *ctx);

// Adds a color stop with the given color to the gradient at the given offset.
// 0.0 is the offset at one end of the gradient, 1.0 is the offset at the other
// end.
CANVASAPI void CanvasGradientAddColorStop(CanvasGradient *gradient,
                                          float offset, CanvasColor *color);

// Returns a CanvasGradient object that represents a linear gradient that paints
// along the line given by the coordinates represented by the arguments.
CANVASAPI CanvasGradient *CanvasCreateLinearGradient(
    CanvasRenderingContext2D *ctx, float x0, float y0, float x1, float y1);

// Returns a CanvasGradient object that represents a radial gradient that paints
// along the cone given by the circles represented by the arguments.
CANVASAPI CanvasGradient *CanvasCreateRadialGradient(
    CanvasRenderingContext2D *ctx, float x0, float y0, float r0, float x1,
    float y1, float r1);

typedef struct CanvasImage CanvasImage;
typedef enum CanvasPatternRepetition CanvasPatternRepetition;
enum CanvasPatternRepetition {
  CANVAS_PATTERN_REPETITION_REPEAT,
  CANVAS_PATTERN_REPETITION_REPEAT_X,
  CANVAS_PATTERN_REPETITION_REPEAT_Y,
  CANVAS_PATTERN_REPETITION_NO_REPEAT,
};

CANVASAPI CanvasPattern *CanvasCreatePattern(
    CanvasRenderingContext2D *ctx, CanvasImage *image,
    CanvasPatternRepetition repetition);

CANVASAPI void CanvasPatternSetTransform(CanvasPattern *pattern,
                                         CanvasMatrix2D transform);

// ----------------------------------------------------------------------------
#endif  // CANVAS_CANVAS_H
