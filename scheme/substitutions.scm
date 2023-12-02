(load-from-path "template.scm")

(define make-schema-node
  (lambda* (nodes precursors #:optional rules)
           (let ((nodes-dependencies
                   (map 
                     (lambda (make-schema-node)
                       (make-schema-node nodes))
                     precursors))) 
             (make-dependency-node 
               #:rule rules 
               #:dependent-nodes nodes-dependencies))))


(define make-schema
  (lambda* (#:key rules precursors)
           (lambda* (#:optional nodes)
                    (if precursors
                        (make-schema-node nodes precursors rules)
                        (make-dependency-node #:rule rules #:dependent-nodes nodes)))))

(define double-schema
  (make-schema
    #:rules '((number-type . "double") 
              (math-function-suffix . "")
              (math-macro-suffix . "")
              (function-tag . "_double")
              (struct-type-suffix . "Double"))))

(define float-schema
  (make-schema
    #:rules '((number-type . "float") 
              (math-function-suffix . "f")
              (math-macro-suffix . "F")
              (function-tag . "_float")
              (struct-type-suffix . "Float"))))

(define number-schema
  (make-schema
    #:rules '((number-base-type . "${number-type}")
              (real-struct-type-suffix . "${struct-type-suffix}")
              (real-function-tag . "${function-tag}"))
    #:precursors (list double-schema float-schema)))


(define complex-number-schema
  (make-schema
    #:rules '((number-type . "${number-type} complex") 
              (struct-type-suffix . "Complex${struct-type-suffix}")
              (function-tag . "_complex_${number-base-type}"))
    #:precursors (list number-schema)))

(define real-number-schema
  (make-schema
    #:rules '((number-type . "${number-type}") 
              (struct-type-suffix . "Real${struct-type-suffix}")
              (function-tag . "_real_${number-base-type}"))
    #:precursors (list number-schema)))


(define numeric-schema
  (make-schema
    #:precursors (list complex-number-schema  
                          real-number-schema)))

(define vector-schema
  (make-schema
    #:rules '((vector-type . "Vector${struct-type-suffix}"))
    #:precursors (list numeric-schema)))

(define filter-type-rule
  '(filter-type . "DigitalFilter${struct-type-suffix}"))

(define filter-schema
  (make-schema
    #:rules (list 
              filter-type-rule
              '(window-function-type . "WindowFunction${real-struct-type-suffix}"))
    #:precursors (list vector-schema)))


(define moving-average-type-rule
  '(moving-average-type . "MovingAverage${struct-type-suffix}"))

(define moving-average-schema
  (make-schema
    #:rules (list moving-average-type-rule)
    #:precursors (list vector-schema)))

(define oscillator-type-rule
  '(oscillator-type . "Oscillator${struct-type-suffix}"))

(define oscillator-schema
  (make-schema
    #:rules (list oscillator-type-rule)
    #:precursors (list number-schema)))


(define pid-type-rule
  '(pid-type . "Pid${struct-type-suffix}"))

(define pid-schema
  (make-schema
    #:rules pid-type-rule
    #:precursors (list number-schema)))


(define pll-schema
  (make-schema
    #:rules (list 
              oscillator-type-rule 
              pid-type-rule 
              '(pll-type . "PhaseLockedLoop${struct-type-suffix}"))
    #:precursors (list number-schema)))


(define sinusoid-fit-type-rule
  '(sinusoid-fit-type . "SinusoidFit${struct-type-suffix}"))

(define sinusoid-fit-schema
  (make-schema
    #:rules (list oscillator-type-rule moving-average-type-rule sinusoid-fit-type-rule)
    #:precursors (list number-schema)))

(define window-schema
  (make-schema
    #:rules '((window-function-type . "WindowFunction${struct-type-suffix}"))
    #:precursors (list number-schema)))

(define fft-schema 
  (make-schema
    #:rules '((fft-type . "FftComplex${struct-type-suffix}")
              (vector-type . "VectorComplex${struct-type-suffix}")
              (complex-function-tag . "_complex${function-tag}")
              (complex-number-type . "${number-type} complex"))
    #:precursors (list number-schema)))

(define linear-estimator-schema
  (make-schema
    #:rules (list 
              filter-type-rule 
              moving-average-type-rule
              '(struct-type-suffix . "Real${struct-type-suffix}")
              '(linear-estimator-type . "LinearEstimator${struct-type-suffix}")
              '(linear-model-type . "LinearModel${struct-type-suffix}"))
    #:precursors (list number-schema)))
