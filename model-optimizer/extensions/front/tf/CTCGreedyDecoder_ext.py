# Copyright (C) 2018-2021 Intel Corporation
# SPDX-License-Identifier: Apache-2.0

from extensions.ops.ctc_greedy_decoder_seq_len import CTCGreedyDecoderSeqLenOp
from mo.front.extractor import FrontExtractorOp


class CTCCGreedyDecoderFrontExtractor(FrontExtractorOp):
    op = 'CTCGreedyDecoder'
    enabled = True

    @classmethod
    def extract(cls, node):
        attrs = {
            'merge_repeated': bool(node.pb.attr['merge_repeated'].b),
            'output_sparse_format': True,  # Special argument for TF CTCGreedyDecoder replacement transformations
        }
        CTCGreedyDecoderSeqLenOp.update_node_stat(node, attrs)
        return cls.enabled
