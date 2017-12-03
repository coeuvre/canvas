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
//
// Objects that implement the CanvasState interface maintain a stack of drawing
// states. Drawing states consist of:
//
// - The current transformation matrix.
// - The current clipping region.
// - The current values of the following attributes: strokeStyle, fillStyle,
//   globalAlpha, lineWidth, lineCap, lineJoin, miterLimit, lineDashOffset,
//   shadowOffsetX, shadowOffsetY, shadowBlur, shadowColor, filter,
//   globalCompositeOperation, font, textAlign, textBaseline, direction,
//   imageSmoothingEnabled, imageSmoothingQuality.
// - The current dash list.
//
// NOTE: The current default path and the rendering context's bitmaps are not
// part of the drawing state. The current default path is persistent, and can
// only be reset using the canvasBeginPath() method. The bitmaps depend on
// whether and how the rendering context is bound to a canvas element.

// Pushes the current state onto the stack.
//
// NOTE: When invoked, must push a copy of the current drawing state onto the
// drawing state stack.
CANVASAPI void canvasSave(CanvasRenderingContext2D *ctx);

// Pops the top state on the stack, restoring the context to that state.
//
// NOTE: When invoked, must pop the top entry in the drawing state stack, and
// reset the drawing state it describes. If there is no saved state, then the
// method must do nothing.
CANVASAPI void canvasRestore(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Transformations
//
// Objects that implement the CanvasTransform interface have a current
// transformation matrix, as well as methods (described in this section) to
// manipulate it. When an object implementing the CanvasTransform interface is
// created, its transformation matrix must be initialized to the identity
// transform.
//
// The current transformation matrix is applied to coordinates when creating the
// current default path, and when painting text, shapes, and Path2D objects, on
// objects implementing the CanvasTransform interface.
//
// NOTE: Most of the API uses DOMMatrix objects rather than this API. This API
// remains mostly for historical reasons.
//
// The transformations must be performed in reverse order.
//
// NOTE: For instance, if a scale transformation that doubles the width is
// applied to the canvas, followed by a rotation transformation that rotates
// drawing operations by a quarter turn, and a rectangle twice as wide as it is
// tall is then drawn on the canvas, the actual result will be a square.

typedef struct CanvasMatrix2D CanvasMatrix2D;
struct CanvasMatrix2D {
  float a, b, c, d, e, f;
};

// Changes the current transformation matrix to apply a scaling transformation
// with the given characteristics.
//
// NOTE: When invoked, must run these steps:
// 1. If either of the arguments are infinite or NaN, then return.
// 2. Add the scaling transformation described by the arguments to the current
//    transformation matrix. The x argument represents the scale factor in the
//    horizontal direction and the y argument represents the scale factor in the
//    vertical direction. The factors are multiples.
CANVASAPI void canvasScale(CanvasRenderingContext2D *ctx, float x, float y);

// Changes the current transformation matrix to apply a rotation transformation
// with the given characteristics. The angle is in radians.
//
// NOTE: When invoked, must run these steps:
// 1. If angle is infinite or NaN, then return.
// 2. Add the rotation transformation described by the argument to the current
//    transformation matrix. The angle argument represents a clockwise rotation
//    angle expressed in radians.
CANVASAPI void canvasRotate(CanvasRenderingContext2D *ctx, float angle);

// Changes the current transformation matrix to apply a translation
// transformation with the given characteristics.
//
// NOTE: When invoked, must run these steps:
// 1. If either of the arguments are infinite or NaN, then return.
// 2. Add the translation transformation described by the arguments to the
//    current transformation matrix. The x argument represents the translation
//    distance in the horizontal direction and the y argument represents the
//    translation distance in the vertical direction. The arguments are in
//    coordinate space units.
CANVASAPI void canvasTranslate(CanvasRenderingContext2D *ctx, float x, float y);

// Changes the current transformation matrix to apply the matrix given by the
// arguments as described below.
//
// NOTE: when invoked, must run these steps:
// 1. If any of the arguments are infinite or NaN, then return.
// 2. Replace the current transformation matrix with the result of multiplying
//    the current transformation matrix with the matrix described by:
//                             a c e
//                             b d f
//                             0 0 1
CANVASAPI void canvasTransform(CanvasRenderingContext2D *ctx, float a, float b,
                               float c, float d, float e, float f);

// Returns a copy of the current transformation matrix, as a newly created
// DOMMatrix object.
//
// NOTE: When invoked, must return a newly created DOMMatrix representing a
// copy of the current transformation matrix matrix of the context.
CANVASAPI CanvasMatrix2D canvasGetTransform(CanvasRenderingContext2D *ctx);

// Changes the current transformation matrix to the matrix given by the
// arguments as described below.
//
// NOTE: when invoked, must run these steps:
// 1. If any of the arguments are infinite or NaN, then return.
// 2. Reset the current transformation matrix to the identity matrix.
// 3. Invoke the canvasTransform(a, b, c, d, e, f) method with the same
//    arguments.
CANVASAPI void canvasSetTransform(CanvasRenderingContext2D *ctx, float a,
                                  float b, float c, float d, float e, float f);

// Changes the current transformation matrix to the matrix represented by the
// passed DOMMatrix2DInit object.
//
// NOTE: When invoked, must run these steps:
// 1. Let matrix be the result of creating a DOMMatrix from the 2D dictionary
//    transform.
// 2. If one or more of matrix's m11 element, m12 element, m21 element, m22
//    element, m41 element, or m42 element are infinite or NaN, then return.
// 3. Reset the current transformation matrix to matrix.
CANVASAPI void canvasSetTransformMatrix2D(CanvasRenderingContext2D *ctx,
                                          CanvasMatrix2D matrix);

// Changes the current transformation matrix to the identity transform.
//
// NOTE: When invoked, must reset the current transformation matrix to the
// identity matrix.
CANVASAPI void canvasResetTransform(CanvasRenderingContext2D *ctx);

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Compositing
//
// All drawing operations on an object which implements the CanvasCompositing
// interface are affected by the global compositing attributes, globalAlpha and
// globalCompositeOperation.
//
// The globalAlpha attribute gives an alpha value that is applied to shapes and
// images before they are composited onto the output bitmap. The value must be
// in the range from 0.0 (fully transparent) to 1.0 (no additional
// transparency). If an attempt is made to set the attribute to a value outside
// this range, including Infinity and Not-a-Number (NaN) values, then the
// attribute must retain its previous value. When the context is created, the
// globalAlpha attribute must initially have the value 1.0.
//
// The globalCompositeOperation attribute sets the current composition operator,
// which controls how shapes and images are drawn onto the output bitmap, once
// they have had globalAlpha and the current transformation matrix applied. The
// possible values are those defined in the Compositing and Blending
// specification, and include the values source-over and copy.
//
// These values are all case-sensitive — they must be used exactly as defined.
// User agents must not recognize values that are not a case-sensitive match for
// one of the values given in the Compositing and Blending specification.
//
// On setting, if the user agent does not recognize the specified value, it must
// be ignored, leaving the value of globalCompositeOperation unaffected.
// Otherwise, the attribute must be set to the given new value.
//
// When the context is created, the globalCompositeOperation attribute must
// initially have the value source-over.

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
//
// Objects that implement the CanvasImageSmoothing interface have attributes
// that control how image smoothing is performed.
//
// The imageSmoothingEnabled attribute, on getting, must return the last value
// it was set to. On setting, it must be set to the new value. When the object
// implementing the CanvasImageSmoothing interface is created, the attribute
// must be set to true.
//
// The imageSmoothingQuality attribute, on getting, must return the last value
// it was set to. On setting, it must be set to the new value. When the object
// implementing the CanvasImageSmoothing interface is created, the attribute
// must be set to "low".

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

#endif  // CANVAS_CANVAS_H
