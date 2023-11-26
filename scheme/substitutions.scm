(load-from-path "template.scm")

(define make-schema
  (lambda* (#:key rules prerequisites)
    (lambda* (#:optional edges)
      (make-schema-node 
        #:rule rules 
        #:prerequisites (if prerequisites
                            (map 
                              (lambda (prerequisite)
                                (prerequisite edges))
                              prerequisites)
                            edges)))))

(define double-schema
  (make-schema
    #:rules '((number-base-type . "double") 
              (struct-type-precision . "Double"))))

(define float-schema
  (make-schema
    #:rules '((number-base-type . "float") 
              (struct-type-precision . "Float"))))

(define number-schema
  (make-schema
    #:prerequisites (list double-schema float-schema)))

(define complex-number-schema
  (make-schema
    #:rules '((number-type . "${number-base-type} complex") 
              (struct-type-suffix . "Complex${struct-type-precision}")
              (function-tag . "_complex_${number-base-type}"))
    #:prerequisites (list number-schema)))

(define real-number-schema
  (make-schema
    #:rules '((number-type . "${number-base-type}") 
              (struct-type-suffix . "Real${struct-type-precision}")
              (function-tag . "_real_${number-base-type}"))
    #:prerequisites (list number-schema)))

(define numeric-schema
  (make-schema
    #:prerequisites (list complex-number-schema  
                          real-number-schema)))

(define vector-schema
  (make-schema
    #:rules '((vector-type . "Vector${struct-type-suffix}"))
    #:prerequisites (list numeric-schema)))
