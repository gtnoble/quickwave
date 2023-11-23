(load-from-path "template.scm")

(define make-number-substitution
    (lambda (type-tag number-type real-number-type function-tag documentation-number-type)
        (make-substitution 
            type-tag
            (list
                (make-rule 'number-type number-type) 
                (make-rule 'real-number-type real-number-type)
                (make-rule 'function-tag function-tag)
                (make-rule 'documentation-number-type documentation-number-type)))))

(define make-real-number-substitution
    (lambda (type-tag number-type function-tag documentation-number-type)
        (make-substitution 
            type-tag
            (list
                (make-rule 'number-type number-type)
                (make-rule 'function-tag function-tag)
                (make-rule 'documentation-number-type documentation-number-type)))))

(define real-number-substitutions
    (list
        (make-real-number-substitution 'double "double" "_real_double" "real double-precision" )
        (make-real-number-substitution 'float "float" "_real_float" "real single-precision")))

(define complex-number-substitutions
    (list 
        (make-complex-number-substitution 'double "double" "double" "_real_double" "real double-precision")
        (make-complex-number-substitution 'complex-double "double complex" "double" "_complex_double" "complex double-precision")
        (make-complex-number-substitution 'float "float" "float" "_real_float" "real single-precision")
        (make-complex-number-substitution 'complex-float "float complex" "float" "_complex_float" "complex single-precision")))

(define make-vector-substitution
    (lambda (type-tag vector-type)
        (make-substitution 
            type-tag 
            (list (make-rule 'vector-type vector-type)))))
            

(define vector-substitutions
    (list 
        (make-vector-substitution 'double "VectorRealDouble")
        (make-vector-substitution 'complex-double "VectorComplexDouble")
        (make-vector-substitution 'float "VectorRealFloat")
        (make-vector-substitution 'complex-float "VectorComplexFloat")))


