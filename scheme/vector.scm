(load "template.scm")

(define make-vector-replacement
    (lambda (element-type vector-type)
        (list 
            (cons "element-type" element-type) 
            (cons "vector-type" vector-type))))

(define vector-replacements
    (list 
        (make-vector-replacement "double" "VectorRealDouble")
        (make-vector-replacement "double complex" "VectorComplexDouble")
        (make-vector-replacement "float" "VectorRealFloat")
        (make-vector-replacement "float complex" "VectorComplexFloat")))

(define vector-shift-prototype-template
"${element-type} vector_complex_shift(${element-type} element, ${vector-type} *buf)"
)

(define vector-shift-template
"
${prototype} {
    assert(buf != NULL);
    assert(buf->elements != NULL);
    buf->last_element_index = modular_add(buf->last_element_index, 1, buf->n_elements);
    ${element-type} last_element = buf->elements[buf->last_element_index];
    buf->elements[buf->last_element_index] = element;
    return last_element;
}
")



